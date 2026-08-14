#include "core/mesh/deformer_graph.hpp"
#include "core/mesh/mesh.hpp"

#include <iomanip>
#include <iostream>

int main()
{
    using namespace otuber::mesh;

    Mesh mesh = make_grid(4, 4);
    DeformerGraph graph;

    // A small demonstration region: the upper half of the grid is rotated
    // while the lower half remains fixed. This exercises the same core path
    // that the eventual visual preview will use.
    DeformationRegion upper_half;
    for (std::uint32_t i = 0; i < mesh.vertices.size(); ++i) {
        if (mesh.vertices[i].y <= 0.0f)
            upper_half.influences.push_back({i, 1.0f});
    }

    RegionTransform tilt;
    tilt.rotation = 0.20f;
    tilt.translate_x = 0.10f;

    graph.add({"preview_tilt", upper_half, tilt, 1.0f, true});

    const Mesh result = graph.evaluate(mesh);

    std::cout << "Otuber Preview Core\n";
    std::cout << "vertices=" << result.vertices.size()
              << " triangles=" << result.triangles.size() << "\n";

    std::cout << std::fixed << std::setprecision(3);
    for (std::size_t i = 0; i < result.vertices.size(); ++i) {
        const auto &v = result.vertices[i];
        std::cout << "v[" << i << "] = (" << v.x << ", " << v.y
                  << ") uv=(" << v.u << ", " << v.v << ")\n";
    }

    return 0;
}
