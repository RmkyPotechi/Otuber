#pragma once

#include <string>
#include <vector>

namespace otuber::core {

enum class AvatarPartType {
    Unknown,
    Body,
    Head,
    Eye,
    Mouth,
    Hair,
    Accessory,
};

struct AvatarPart {
    std::string id;
    AvatarPartType type = AvatarPartType::Unknown;
    bool visible = true;
};

struct AvatarParameter {
    std::string id;
    float value = 0.0f;
    float minimum = -1.0f;
    float maximum = 1.0f;
};

class AvatarModel {
public:
    void add_part(AvatarPart part);
    void add_parameter(AvatarParameter parameter);

    [[nodiscard]] const std::vector<AvatarPart>& parts() const noexcept;
    [[nodiscard]] const std::vector<AvatarParameter>& parameters() const noexcept;

private:
    std::vector<AvatarPart> parts_;
    std::vector<AvatarParameter> parameters_;
};

} // namespace otuber::core
