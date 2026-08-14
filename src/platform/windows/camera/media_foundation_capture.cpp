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

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    const bool com_ok = SUCCEEDED(hr) || hr == RPC_E_CHANGED_MODE;
    if (!com_ok)
        return false;

    hr = MFStartup(MF_VERSION, MFSTARTUP_FULL);
    initialized_ = SUCCEEDED(hr);
    if (!initialized_ && hr == S_OK && com_ok)
        CoUninitialize();
    return initialized_;
#else
    return false;
#endif
}

bool MediaFoundationCapture::open(int device_index)
{
#ifdef _WIN32
    if (!initialized_ || device_index < 0)
        return false;

    shutdown();
    // shutdown() also tears down MF, so restore the initialized state.
    if (!initialize())
        return false;

    ComPtr<IMFAttributes> attributes;
    if (FAILED(MFCreateAttributes(&attributes, 1)))
        return false;

    if (FAILED(attributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                                   MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID)))
        return false;

    IMFActivate **devices = nullptr;
    UINT32 count = 0;
    HRESULT hr = MFEnumDeviceSources(attributes.Get(), &devices, &count);
    if (FAILED(hr) || static_cast<UINT32>(device_index) >= count) {
        if (devices)
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
    if (FAILED(MFCreateAttributes(&reader_attributes, 1)))
        return false;
    reader_attributes->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, FALSE);

    ComPtr<IMFSourceReader> reader;
    hr = MFCreateSourceReaderFromMediaSource(source.Get(), reader_attributes.Get(), &reader);
    if (FAILED(hr))
        return false;

    ComPtr<IMFMediaType> type;
    if (FAILED(MFCreateMediaType(&type)))
        return false;
    type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
    type->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);

    // Request a modest default capture size. The source reader may choose a
    // compatible native mode when this exact size is unavailable.
    MFSetAttributeSize(type.Get(), MF_MT_FRAME_SIZE, 640, 480);
    hr = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                      nullptr, type.Get());
    if (FAILED(hr))
        return false;

    ComPtr<IMFMediaType> current_type;
    if (FAILED(reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                           &current_type)))
        return false;

    UINT32 width = 0, height = 0;
    if (FAILED(MFGetAttributeSize(current_type.Get(), MF_MT_FRAME_SIZE, &width, &height)))
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
        CoUninitialize();
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
    DWORD max_length = 0, current_length = 0;
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

    // RGB32 is stored as B,G,R,X on Windows. Convert to RGBA for the core.
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
