#include "camera_tracker.hpp"

#include <algorithm>

namespace otuber::motion {

CameraState CameraTracker::process(const CameraFrame &frame) const
{
    CameraState state{};

    if (frame.width <= 0 || frame.height <= 0 || frame.pixels == nullptr)
        return state;

    // Capture plumbing is intentionally separated from landmark inference.
    // For now, receiving a valid frame establishes the connected state.
    state.connected = true;
    state.head_x = 0.0f;
    state.head_y = 0.0f;
    state.head_rotation = 0.0f;
    state.eye_left_open = 1.0f;
    state.eye_right_open = 1.0f;
    state.mouth_open = 0.0f;

    return state;
}

} // namespace otuber::motion
