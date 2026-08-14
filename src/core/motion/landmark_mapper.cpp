#include "landmark_mapper.hpp"

#include <algorithm>
#include <cmath>

namespace otuber::motion {
namespace {

float clamp01(float value)
{
    return std::clamp(value, 0.0f, 1.0f);
}

float distance(Point a, Point b)
{
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

} // namespace

CameraState LandmarkMapper::map(const FaceLandmarks &landmarks) const
{
    CameraState state{};
    state.connected = landmarks.detected;
    if (!landmarks.detected)
        return state;

    // Normalized coordinates are expected to be in [0, 1].
    state.head_x = (landmarks.nose.x - 0.5f) * 2.0f;
    state.head_y = (landmarks.nose.y - 0.5f) * 2.0f;

    const float eye_y = (landmarks.left_eye.y + landmarks.right_eye.y) * 0.5f;
    const float eye_dx = landmarks.right_eye.x - landmarks.left_eye.x;
    const float eye_dy = landmarks.right_eye.y - landmarks.left_eye.y;
    state.head_rotation = std::atan2(eye_dy, eye_dx);

    const float mouth_height = distance(landmarks.mouth_top, landmarks.mouth_bottom);
    const float mouth_width = std::max(distance(landmarks.mouth_left, landmarks.mouth_right), 0.001f);
    state.mouth_open = clamp01((mouth_height / mouth_width) * 4.0f);

    // With only a compact landmark set, eye openness is estimated from the
    // eye-to-mouth geometry. A real landmark backend can supply richer eyelid
    // points later without changing CameraState or the mapper API.
    const float left_eye_mouth = distance(landmarks.left_eye, landmarks.mouth_top);
    const float right_eye_mouth = distance(landmarks.right_eye, landmarks.mouth_top);
    state.eye_left_open = clamp01(left_eye_mouth * 2.0f);
    state.eye_right_open = clamp01(right_eye_mouth * 2.0f);

    (void)eye_y;
    return state;
}

} // namespace otuber::motion
