#pragma once

#include "core/motion/camera_state.hpp"

namespace otuber::preview {

struct AppState {
    otuber::motion::CameraState motion;
    bool camera_enabled = false;
    bool tracking_enabled = true;
    bool show_camera = true;
    bool show_mesh = false;
};

} // namespace otuber::preview
