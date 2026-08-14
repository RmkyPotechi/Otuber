#pragma once

namespace otuber::motion {

struct CameraState {
    bool connected = false;
    float head_x = 0.0f;
    float head_y = 0.0f;
    float head_rotation = 0.0f;
    float eye_left_open = 1.0f;
    float eye_right_open = 1.0f;
    float mouth_open = 0.0f;
};

} // namespace otuber::motion
