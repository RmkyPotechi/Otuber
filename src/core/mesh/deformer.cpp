#include "deformer.hpp"

#include <cmath>

namespace otuber::mesh {

Mesh apply_deformation(const Mesh &source, const Deformation &deformation)
{
    Mesh result = source;
    const float c = std::cos(deformation.rotation);
    const float s = std::sin(deformation.rotation);

    for (auto &vertex : result.vertices) {
        const float x = vertex.x * deformation.scale_x;
        const float y = vertex.y * deformation.scale_y;

        vertex.x = x * c - y * s + deformation.translate_x;
        vertex.y = x * s + y * c + deformation.translate_y;
    }

    return result;
}

} // namespace otuber::mesh
