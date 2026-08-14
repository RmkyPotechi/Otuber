#pragma once

#include "avatar_instance.hpp"
#include "core/motion/camera_state.hpp"

namespace otuber::avatar {

class MotionAvatarController {
public:
    void apply(const otuber::motion::CameraState &state, AvatarInstance &avatar) const;
};

} // namespace otuber::avatar
