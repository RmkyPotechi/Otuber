#pragma once

namespace otuber::motion {

struct Point {
    float x = 0.0f;
    float y = 0.0f;
};

struct FaceLandmarks {
    bool detected = false;
    Point nose;
    Point left_eye;
    Point right_eye;
    Point mouth_left;
    Point mouth_right;
    Point mouth_top;
    Point mouth_bottom;
};

} // namespace otuber::motion
