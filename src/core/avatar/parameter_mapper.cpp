#include "parameter_mapper.hpp"

#include <algorithm>

namespace otuber::avatar {
namespace {

float remap(float value, float in_min, float in_max, float out_min, float out_max)
{
    if (in_min == in_max)
        return out_min;

    const float t = std::clamp((value - in_min) / (in_max - in_min), 0.0f, 1.0f);
    return out_min + (out_max - out_min) * t;
}

} // namespace

float ParameterMapper::map_value(const ParameterMapping &mapping, float input)
{
    const float mapped = remap(input,
                               mapping.input_minimum,
                               mapping.input_maximum,
                               mapping.output_minimum,
                               mapping.output_maximum);
    return mapped * mapping.multiplier;
}

void ParameterMapper::apply(AvatarModel &model, const ParameterMapping &mapping, float input)
{
    for (auto &parameter : model.parameters) {
        if (parameter.id != mapping.parameter_id)
            continue;

        parameter.value = std::clamp(map_value(mapping, input),
                                     parameter.minimum,
                                     parameter.maximum);
        return;
    }
}

} // namespace otuber::avatar
