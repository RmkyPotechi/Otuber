#pragma once

#include "avatar_model.hpp"
#include "core/mesh/deformer_graph.hpp"
#include "core/texture/texture.hpp"

namespace otuber::avatar {

struct AvatarInstance {
    AvatarModel model;
    otuber::texture::Texture texture;
    otuber::mesh::Mesh mesh;
    otuber::mesh::DeformerGraph deformers;

    otuber::mesh::Mesh evaluate_mesh() const;
};

} // namespace otuber::avatar
