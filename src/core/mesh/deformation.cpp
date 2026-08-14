#include "deformation.hpp"

#include <algorithm>
#include <cmath>

namespace otuber::mesh {

Mesh apply_region_transform(const Mesh &source,
                            const DeformationRegion &region,
                            const RegionTransform &transform)
{
    Mesh result = source;
    const float c = std::cos(transform.rotation);
    const float s = std::sin(transform.rotation);

    for (const auto &influence : region.influences) {
        if (influence.vertex >= result.vertices.size())
            continue;

        const float weight = std::clamp(influence.weight, 0.0f, 1.0f);
        auto &vertex = result.vertices[influence.vertex];

        const float x = vertex.x * transform.scale_x;
        const float y = vertex.y * transform.scale_y;
        const float target_x = x * c - y * s + transform.translate_x;
        const float target_y = x * s + y * c + transform.translate_y;

        vertex.x += (target_x - vertex.x) * weight;
        vertex.y += (target_y - vertex.y) * weight;
    }

    return result;
}

} // namespace otuber::mesh
