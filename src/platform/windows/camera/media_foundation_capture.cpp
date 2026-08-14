#include "media_foundation_capture.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <combaseapi.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;
#endif

namespace otuber::windows::camera {

MediaFoundationCapture::~MediaFoundationCapture()
{
    shutdown();
}

void MediaFoundationCapture::set_error(const char *message)
{
    last_error_ = message ? message : "unknown camera error";
}

#ifdef _WIN32
void MediaFoundationCapture::set_hresult_error(const char *operation, long hr)
{
    char buffer[96]{};
    std::snprintf(buffer, sizeof(buffer), "%s (HRESULT 0x%08lX)",
                  operation ? operation : "camera operation failed",
                  static_cast<unsigned long>(hr));
    last_error_ = buffer;
}
#endif

bool MediaFoundationCapture::initialize()
{
#ifdef _WIN32
    if (initialized_)
        return true;

    last_error_.clear();
    const HRESULT com_hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(com_hr) && com_hr != RPC_E_CHANGED_MODE) {
        set_hresult_error("CoInitializeEx failed", com_hr);
        return false;
    }

    com_initialized_ = SUCCEEDED(com_hr);

    const HRESULT mf_hr = MFStartup(MF_VERSION, MFSTARTUP_FULL);
    if (FAILED(mf_hr)) {
        set_hresult_error("MFStartup failed", mf_hr);
        if (com_initialized_) {
            CoUninitialize();
            com_initialized_ = false;
        }
        return false;
    }

    initialized_ = true;
    return true;
#else
    set_error("Windows Media Foundation is unavailable");
    return false;
#endif
}

bool MediaFoundationCapture::open(int device_index)
{
#ifdef _WIN32
    if (device_index < 0) {
        set_error("Invalid camera index");
        return false;
    }

    if (!initialized_ && !initialize())
        return false;

    if (opened_)
        shutdown();

    if (!initialized_ && !initialize())
        return false;

    ComPtr<IMFAttributes> attributes;
    HRESULT hr = MFCreateAttributes(&attributes, 2);
    if (FAILED(hr)) {
        set_hresult_error("MFCreateAttributes failed", hr);
        return false;
    }

    hr = attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                             MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    if (FAILED(hr)) {
        set_hresult_error("Camera device filter setup failed", hr);
        return false;
    }

    IMFActivate **devices = nullptr;
    UINT32 count = 0;
    hr = MFEnumDeviceSources(attributes.Get(), &devices, &count);
    if (FAILED(hr)) {
        set_hresult_error("MFEnumDeviceSources failed", hr);
        return false;
    }

    if (static_cast<UINT32>(device_index) >= count) {
        for (UINT32 i = 0; i < count; ++i)
            devices[i]->Release();
        CoTaskMemFree(devices);
        set_error(count == 0 ? "No camera devices found" : "Camera index is out of range");
        return false;
    }

    ComPtr<IMFMediaSource> source;
    hr = devices[device_index]->ActivateObject(IID_PPV_ARGS(&source));
    for (UINT32 i = 0; i < count; ++i)
        devices[i]->Release();
    CoTaskMemFree(devices);
    if (FAILED(hr)) {
        set_hresult_error("Camera activation failed", hr);
        return false;
    }

    ComPtr<IMFAttributes> reader_attributes;
    hr = MFCreateAttributes(&reader_attributes, 3);
    if (FAILED(hr)) {
        set_hresult_error("SourceReader attributes failed", hr);
        return false;
    }

    // Ask SourceReader to perform video processing/conversion. This is
    // important for webcams that expose YUY2/NV12 instead of RGB32.
    reader_attributes->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, FALSE);
    reader_attributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);

    ComPtr<IMFSourceReader> reader;
    hr = MFCreateSourceReaderFromMediaSource(source.Get(), reader_attributes.Get(), &reader);
    if (FAILED(hr)) {
        set_hresult_error("SourceReader creation failed", hr);
        return false;
    }

    ComPtr<IMFMediaType> requested_type;
    hr = MFCreateMediaType(&requested_type);
    if (FAILED(hr)) {
        set_hresult_error("Media type creation failed", hr);
        return false;
    }

    requested_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    requested_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);

    hr = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                     nullptr, requested_type.Get());
    if (FAILED(hr)) {
        set_hresult_error("RGB32 camera format negotiation failed", hr);
        return false;
    }

    ComPtr<IMFMediaType> current_type;
    hr = reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                     &current_type);
    if (FAILED(hr)) {
        set_hresult_error("Reading negotiated camera format failed", hr);
        return false;
    }

    UINT32 width = 0;
    UINT32 height = 0;
    hr = MFGetAttributeSize(current_type.Get(), MF_MT_FRAME_SIZE, &width, &height);
    if (FAILED(hr) || width == 0 || height == 0) {
        set_hresult_error("Camera frame size is unavailable", hr);
        return false;
    }

    reader_ = reader.Detach();
    source_ = source.Detach();
    width_ = static_cast<int>(width);
    height_ = static_cast<int>(height);
    opened_ = true;
    last_error_.clear();
    return true;
#else
    (void)device_index;
    set_error("Windows Media Foundation is unavailable");
    return false;
#endif
}

void MediaFoundationCapture::shutdown()
{
#ifdef _WIN32
    opened_ = false;
    if (reader_) {
        reader_->Release();
        reader_ = nullptr;
    }
    if (source_) {
        source_->Shutdown();
        source_->Release();
        source_ = nullptr;
    }
    width_ = height_ = 0;

    if (initialized_) {
        MFShutdown();
        initialized_ = false;
    }
    if (com_initialized_) {
        CoUninitialize();
        com_initialized_ = false;
    }
#else
    opened_ = false;
    initialized_ = false;
#endif
}

bool MediaFoundationCapture::read(CapturedFrame &frame)
{
#ifdef _WIN32
    if (!opened_ || !reader_) {
        set_error("Camera is not open");
        return false;
    }

    DWORD stream_flags = 0;
    ComPtr<IMFSample> sample;
    LONGLONG timestamp = 0;
    HRESULT hr = reader_->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                     0, nullptr, &stream_flags, &timestamp, &sample);
    if (FAILED(hr)) {
        set_hresult_error("Camera frame read failed", hr);
        return false;
    }
    if (stream_flags & MF_SOURCE_READERF_ENDOFSTREAM) {
        set_error("Camera stream ended");
        return false;
    }
    if (!sample || (stream_flags & MF_SOURCE_READERF_STREAMTICK))
        return false;

    ComPtr<IMFMediaBuffer> buffer;
    if (FAILED(sample->ConvertToContiguousBuffer(&buffer))) {
        set_error("Camera sample conversion failed");
        return false;
    }

    BYTE *data = nullptr;
    DWORD max_length = 0;
    DWORD current_length = 0;
    if (FAILED(buffer->Lock(&data, &max_length, &current_length)) || !data) {
        set_error("Camera buffer lock failed");
        return false;
    }

    const std::size_t expected = static_cast<std::size_t>(width_) *
                                 static_cast<std::size_t>(height_) * 4;
    if (current_length < expected) {
        buffer->Unlock();
        set_error("Camera returned an unexpected frame size");
        return false;
    }

    frame.width = width_;
    frame.height = height_;
    frame.rgba.resize(expected);

    // RGB32 is B,G,R,X on Windows. Convert to RGBA for the core.
    for (std::size_t i = 0; i < expected; i += 4) {
        frame.rgba[i + 0] = data[i + 2];
        frame.rgba[i + 1] = data[i + 1];
        frame.rgba[i + 2] = data[i + 0];
        frame.rgba[i + 3] = 255;
    }

    buffer->Unlock();
    last_error_.clear();
    return true;
#else
    (void)frame;
    set_error("Windows Media Foundation is unavailable");
    return false;
#endif
}

} // namespace otuber::windows::camera
