#pragma once

#include "camera_state.hpp"

namespace otuber::motion {

struct CameraFrame {
    int width = 0;
    int height = 0;
    const unsigned char *pixels = nullptr;
};

class CameraTracker {
public:
    CameraState process(const CameraFrame &frame) const;
};

} // namespace otuber::motion
