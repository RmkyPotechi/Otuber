#pragma once

#include "core/motion/camera_tracker.hpp"

namespace otuber::windows::camera {

class CameraCapture {
public:
    bool open(int device_index = 0);
    void close();
    bool is_open() const { return opened_; }

    // Backend-independent frame acquisition hook.
    bool capture(otuber::motion::CameraFrame &frame);

private:
    bool opened_ = false;
};

} // namespace otuber::windows::camera
