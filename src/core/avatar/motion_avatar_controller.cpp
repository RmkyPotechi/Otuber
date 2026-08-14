#include "motion_avatar_controller.hpp"

#include <algorithm>

namespace otuber::avatar {

void MotionAvatarController::apply(const otuber::motion::CameraState &state,
                                    AvatarInstance &avatar) const
{
    const float head_x = std::clamp(state.head_x, -1.0f, 1.0f);
    const float head_y = std::clamp(state.head_y, -1.0f, 1.0f);
    const float rotation = std::clamp(state.head_rotation, -1.2f, 1.2f);
    const float mouth = std::clamp(state.mouth_open, 0.0f, 1.0f);
    const float eye_left = std::clamp(state.eye_left_open, 0.0f, 1.0f);
    const float eye_right = std::clamp(state.eye_right_open, 0.0f, 1.0f);

    avatar.deformers.set_transform("head", {
        head_x * 0.10f,
        head_y * 0.06f,
        rotation * 0.35f,
        1.0f,
        1.0f
    });

    avatar.deformers.set_weight("mouth", mouth);
    avatar.deformers.set_weight("eye_left", 1.0f - eye_left);
    avatar.deformers.set_weight("eye_right", 1.0f - eye_right);
}

} // namespace otuber::avatar
