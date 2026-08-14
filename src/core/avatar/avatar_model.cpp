#include "core/avatar/avatar_model.hpp"

namespace otuber::core {

void AvatarModel::add_part(AvatarPart part) {
    parts_.push_back(std::move(part));
}

void AvatarModel::add_parameter(AvatarParameter parameter) {
    parameters_.push_back(std::move(parameter));
}

const std::vector<AvatarPart>& AvatarModel::parts() const noexcept {
    return parts_;
}

const std::vector<AvatarParameter>& AvatarModel::parameters() const noexcept {
    return parameters_;
}

} // namespace otuber::core
