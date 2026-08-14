#include "core/mesh/deformer_graph.hpp"

#include <cassert>
#include <cmath>

int main()
{
    using namespace otuber::mesh;

    Mesh mesh = make_grid(1, 1);
    DeformerGraph graph;

    DeformationRegion region;
    region.influences.push_back({0, 1.0f});

    RegionTransform move;
    move.translate_x = 0.5f;

    graph.add({"move", region, move, 1.0f, true});

    const Mesh result = graph.evaluate(mesh);
    assert(result.vertices.size() == mesh.vertices.size());
    assert(std::abs(result.vertices[0].x - (mesh.vertices[0].x + 0.5f)) < 0.0001f);

    // Disabled nodes must leave the mesh unchanged.
    graph.add({"disabled", region, RegionTransform{1.0f, 0.0f, 0.0f, 1.0f, 1.0f}, 1.0f, false});
    const Mesh second = graph.evaluate(mesh);
    assert(std::abs(second.vertices[0].x - (mesh.vertices[0].x + 0.5f)) < 0.0001f);

    return 0;
}
