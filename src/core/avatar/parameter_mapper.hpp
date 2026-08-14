#pragma once

#include "avatar_format.hpp"

namespace otuber::avatar {

class ParameterMapper {
public:
    static float map_value(const ParameterMapping &mapping, float input);
    static void apply(AvatarModel &model, const ParameterMapping &mapping, float input);
};

} // namespace otuber::avatar
