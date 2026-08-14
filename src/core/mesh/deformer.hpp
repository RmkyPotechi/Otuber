#pragma once

#include "mesh.hpp"

namespace otuber::mesh {

struct Deformation {
    float translate_x = 0.0f;
    float translate_y = 0.0f;
    float rotation = 0.0f;
    float scale_x = 1.0f;
    float scale_y = 1.0f;
};

Mesh apply_deformation(const Mesh &source, const Deformation &deformation);

} // namespace otuber::mesh
