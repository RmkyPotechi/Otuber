#include "runtime.hpp"

#ifdef OTUBER_HAS_OPENCV
#include "../../../src/platform/windows/tracking/opencv_face_tracker.hpp"
#endif

namespace otuber::preview {

bool Runtime::start(int camera_index)
{
    stop();
    if (!camera_.initialize() || !camera_.open(camera_index)) {
        camera_.shutdown();
        return false;
    }

#ifdef OTUBER_HAS_OPENCV
    auto tracker = std::make_unique<otuber::windows::tracking::OpenCvFaceTracker>(
        "haarcascade_frontalface_default.xml");
    if (tracker->ready())
        tracker_ = std::move(tracker);
#endif

    state_.camera_enabled = true;
#ifdef OTUBER_HAS_OPENCV
    state_.tracking_enabled = tracker_ != nullptr;
#else
    state_.tracking_enabled = false;
#endif
    return true;
}

void Runtime::stop()
{
#ifdef OTUBER_HAS_OPENCV
    tracker_.reset();
#endif
    camera_.shutdown();
    frame_ = {};
    state_.camera_enabled = false;
    state_.tracking_enabled = false;
    state_.motion = {};
}

bool Runtime::tick()
{
    if (!camera_.is_open())
        return false;
    if (!camera_.read(frame_))
        return false;

#ifdef OTUBER_HAS_OPENCV
    if (tracker_) {
        otuber::motion::CameraFrame core_frame{frame_.width, frame_.height, frame_.rgba.data()};
        const auto landmarks = tracker_->detect(core_frame);
        state_.motion = mapper_.map(landmarks);
        state_.tracking_enabled = landmarks.detected;
    }
#endif
    return true;
}

} // namespace otuber::preview
