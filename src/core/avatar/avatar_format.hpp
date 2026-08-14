#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace otuber::avatar {

enum class PartType {
    Unknown,
    Body,
    Head,
    Eye,
    Mouth,
    Hair,
    Accessory,
    Custom,
};

struct AvatarPart {
    std::string id;
    PartType type = PartType::Unknown;
    int mesh_region = -1;
};

struct AvatarParameter {
    std::string id;
    float minimum = -1.0f;
    float maximum = 1.0f;
    float default_value = 0.0f;
    float value = 0.0f;
};

struct ParameterMapping {
    std::string input_id;
    std::string parameter_id;
    float input_minimum = -1.0f;
    float input_maximum = 1.0f;
    float output_minimum = -1.0f;
    float output_maximum = 1.0f;
    float multiplier = 1.0f;
    float smoothing = 0.0f;
};

struct AvatarModel {
    std::uint32_t format_version = 1;
    std::string name;
    std::vector<AvatarPart> parts;
    std::vector<AvatarParameter> parameters;
    std::vector<ParameterMapping> mappings;
};

} // namespace otuber::avatar
