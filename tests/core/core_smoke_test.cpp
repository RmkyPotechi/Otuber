#include "core/avatar/avatar_format.hpp"
#include "core/avatar/parameter_mapper.hpp"
#include "core/generation/generation_pipeline.hpp"
#include "core/mesh/deformation.hpp"
#include "core/mesh/mesh.hpp"

#include <cassert>
#include <cmath>

int main()
{
    using namespace otuber;

    avatar::AvatarModel model;
    model.parameters.push_back({"head_rotation", -1.0f, 1.0f, 0.0f, 0.0f});

    avatar::ParameterMapping mapping;
    mapping.input_minimum = -1.0f;
    mapping.input_maximum = 1.0f;
    mapping.output_minimum = -1.0f;
    mapping.output_maximum = 1.0f;
    mapping.parameter_id = "head_rotation";
    avatar::ParameterMapper::apply(model, mapping, 0.5f);
    assert(std::abs(model.parameters[0].value - 0.5f) < 0.0001f);

    const auto grid = mesh::make_grid(2, 2);
    assert(grid.vertices.size() == 9);
    assert(grid.triangles.size() == 8);

    mesh::DeformationRegion region;
    region.influences.push_back({4, 1.0f});
    mesh::RegionTransform transform;
    transform.translate_x = 0.25f;
    const auto deformed = mesh::apply_region_transform(grid, region, transform);
    assert(std::abs(deformed.vertices[4].x - (grid.vertices[4].x + 0.25f)) < 0.0001f);
    assert(std::abs(deformed.vertices[0].x - grid.vertices[0].x) < 0.0001f);

    generation::GenerationRequest request;
    request.source_image = "example.png";
    const auto assets = generation::GenerationPipeline{}.analyze_and_prepare(request);
    assert(assets.assets.size() == 5);

    return 0;
}
