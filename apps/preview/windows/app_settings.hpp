#pragma once

#include <string>

namespace otuber::preview {

struct AppSettings {
    int camera_index = 0;
    bool mirror_camera = true;
    bool show_tracking = true;
    bool show_mesh = false;
    float smoothing = 0.65f;
};

} // namespace otuber::preview
