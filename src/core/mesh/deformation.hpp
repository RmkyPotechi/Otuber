#pragma once

#include "mesh.hpp"

#include <cstdint>
#include <vector>

namespace otuber::mesh {

struct VertexInfluence {
    std::uint32_t vertex = 0;
    float weight = 0.0f;
};

struct DeformationRegion {
    std::vector<VertexInfluence> influences;
};

struct RegionTransform {
    float translate_x = 0.0f;
    float translate_y = 0.0f;
    float rotation = 0.0f;
    float scale_x = 1.0f;
    float scale_y = 1.0f;
};

// Applies a weighted local transform to the vertices belonging to a region.
Mesh apply_region_transform(const Mesh &source,
                            const DeformationRegion &region,
                            const RegionTransform &transform);

} // namespace otuber::mesh
