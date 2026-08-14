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

bool MediaFoundationCapture::initialize()
{
#ifdef _WIN32
    if (initialized_)
        return true;

    const HRESULT com_hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(com_hr) && com_hr != RPC_E_CHANGED_MODE)
        return false;

    com_initialized_ = SUCCEEDED(com_hr);

    const HRESULT mf_hr = MFStartup(MF_VERSION, MFSTARTUP_FULL);
    if (FAILED(mf_hr)) {
        if (com_initialized_) {
            CoUninitialize();
            com_initialized_ = false;
        }
        return false;
    }

    initialized_ = true;
    return true;
#else
    return false;
#endif
}

bool MediaFoundationCapture::open(int device_index)
{
#ifdef _WIN32
    if (device_index < 0)
        return false;

    if (!initialized_ && !initialize())
        return false;

    if (opened_)
        shutdown();

    // shutdown() tears down Media Foundation, so restore it when reopening.
    if (!initialized_ && !initialize())
        return false;

    ComPtr<IMFAttributes> attributes;
    if (FAILED(MFCreateAttributes(&attributes, 2)))
        return false;

    if (FAILED(attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                                   MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID)))
        return false;

    IMFActivate **devices = nullptr;
    UINT32 count = 0;
    HRESULT hr = MFEnumDeviceSources(attributes.Get(), &devices, &count);
    if (FAILED(hr))
        return false;

    if (static_cast<UINT32>(device_index) >= count) {
        for (UINT32 i = 0; i < count; ++i)
            devices[i]->Release();
        CoTaskMemFree(devices);
        return false;
    }

    ComPtr<IMFMediaSource> source;
    hr = devices[device_index]->ActivateObject(IID_PPV_ARGS(&source));
    for (UINT32 i = 0; i < count; ++i)
        devices[i]->Release();
    CoTaskMemFree(devices);
    if (FAILED(hr))
        return false;

    ComPtr<IMFAttributes> reader_attributes;
    if (FAILED(MFCreateAttributes(&reader_attributes, 2)))
        return false;
    // Keep SourceReader conversion enabled: webcams commonly expose YUY2/NV12
    // rather than RGB32 natively.
    reader_attributes->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, FALSE);

    ComPtr<IMFSourceReader> reader;
    hr = MFCreateSourceReaderFromMediaSource(source.Get(), reader_attributes.Get(), &reader);
    if (FAILED(hr))
        return false;

    // Do not force a particular resolution. Ask the SourceReader for RGB32 and
    // let it select a supported native camera mode. Forcing 640x480 caused many
    // webcams to reject SetCurrentMediaType even though they were usable.
    ComPtr<IMFMediaType> requested_type;
    if (FAILED(MFCreateMediaType(&requested_type)))
        return false;
    requested_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    requested_type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);

    hr = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                     nullptr, requested_type.Get());
    if (FAILED(hr)) {
        // If RGB32 conversion is unavailable, select the camera's native type
        // and still expose a clear failure rather than producing bad pixels.
        return false;
    }

    ComPtr<IMFMediaType> current_type;
    if (FAILED(reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                           &current_type)))
        return false;

    UINT32 width = 0;
    UINT32 height = 0;
    if (FAILED(MFGetAttributeSize(current_type.Get(), MF_MT_FRAME_SIZE,
                                  &width, &height)) || width == 0 || height == 0)
        return false;

    reader_ = reader.Detach();
    source_ = source.Detach();
    width_ = static_cast<int>(width);
    height_ = static_cast<int>(height);
    opened_ = true;
    return true;
#else
    (void)device_index;
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
    if (!opened_ || !reader_)
        return false;

    DWORD stream_flags = 0;
    ComPtr<IMFSample> sample;
    LONGLONG timestamp = 0;
    HRESULT hr = reader_->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                     0, nullptr, &stream_flags, &timestamp, &sample);
    if (FAILED(hr) || (stream_flags & MF_SOURCE_READERF_ENDOFSTREAM) || !sample)
        return false;
    if (stream_flags & MF_SOURCE_READERF_STREAMTICK)
        return false;

    ComPtr<IMFMediaBuffer> buffer;
    if (FAILED(sample->ConvertToContiguousBuffer(&buffer)))
        return false;

    BYTE *data = nullptr;
    DWORD max_length = 0;
    DWORD current_length = 0;
    if (FAILED(buffer->Lock(&data, &max_length, &current_length)) || !data)
        return false;

    const std::size_t expected = static_cast<std::size_t>(width_) *
                                 static_cast<std::size_t>(height_) * 4;
    if (current_length < expected) {
        buffer->Unlock();
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
    return true;
#else
    (void)frame;
    return false;
#endif
}

} // namespace otuber::windows::camera
