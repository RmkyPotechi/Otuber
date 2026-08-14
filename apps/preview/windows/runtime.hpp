#pragma once

#include "app_state.hpp"
#include "avatar_image.hpp"
#include "core/motion/landmark_mapper.hpp"
#include "core/motion/face_tracker.hpp"
#include "../../../src/platform/windows/camera/media_foundation_capture.hpp"

#include <memory>

namespace otuber::preview {

class Runtime {
public:
    bool start(int camera_index = 0);
    void stop();
    bool tick();

    const AppState &state() const { return state_; }
    const otuber::windows::camera::CapturedFrame &camera_frame() const { return frame_; }
    const AvatarImage &avatar_image() const { return avatar_; }
    bool camera_open() const { return camera_.is_open(); }

    bool load_avatar(const std::wstring &path) { return load_png_wic(path, avatar_); }

private:
    AppState state_;
    otuber::windows::camera::MediaFoundationCapture camera_;
    otuber::windows::camera::CapturedFrame frame_;
    otuber::motion::LandmarkMapper mapper_;
#ifdef OTUBER_HAS_OPENCV
    std::unique_ptr<otuber::motion::FaceTracker> tracker_;
#endif
    AvatarImage avatar_;
};

} // namespace otuber::preview
