#include "avatar_instance.hpp"

namespace otuber::avatar {

otuber::mesh::Mesh AvatarInstance::evaluate_mesh() const
{
    return deformers.evaluate(mesh);
}

} // namespace otuber::avatar
