#pragma once

namespace otuber::core {

struct MotionState {
    float head_x = 0.0f;
    float head_y = 0.0f;
    float head_rotation = 0.0f;
    float eye_left = 1.0f;
    float eye_right = 1.0f;
    float mouth_open = 0.0f;
    float body_x = 0.0f;
    float body_y = 0.0f;
    float body_rotation = 0.0f;
};

} // namespace otuber::core
