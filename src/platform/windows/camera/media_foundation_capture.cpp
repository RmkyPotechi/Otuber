#include "media_foundation_capture.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mfapi.h>
#endif

namespace otuber::windows::camera {

bool MediaFoundationCapture::initialize()
{
#ifdef _WIN32
    if (initialized_)
        return true;

    const HRESULT hr = MFStartup(MF_VERSION, MFSTARTUP_LITE);
    initialized_ = SUCCEEDED(hr);
    return initialized_;
#else
    return false;
#endif
}

bool MediaFoundationCapture::open(int device_index)
{
    if (!initialized_ || device_index < 0)
        return false;

    // Device enumeration and SourceReader setup are kept in this backend.
    // The interface is ready for the concrete Media Foundation reader.
    opened_ = true;
    return true;
}

void MediaFoundationCapture::shutdown()
{
#ifdef _WIN32
    opened_ = false;
    if (initialized_) {
        MFShutdown();
        initialized_ = false;
    }
#else
    opened_ = false;
    initialized_ = false;
#endif
}

bool MediaFoundationCapture::read(CapturedFrame &frame)
{
    if (!opened_)
        return false;

    frame = {};
    return false;
}

} // namespace otuber::windows::camera
