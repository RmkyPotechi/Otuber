#include "camera_capture.hpp"

namespace otuber::windows::camera {

bool CameraCapture::open(int device_index)
{
    // Device enumeration/capture is intentionally isolated here. The first
    // milestone only records a successfully selected logical device.
    opened_ = device_index >= 0;
    return opened_;
}

void CameraCapture::close()
{
    opened_ = false;
}

bool CameraCapture::capture(otuber::motion::CameraFrame &frame)
{
    if (!opened_)
        return false;

    // Real frame acquisition is supplied by the Windows capture backend in
    // the next platform milestone. Never expose an uninitialized frame.
    frame = {};
    return false;
}

} // namespace otuber::windows::camera
