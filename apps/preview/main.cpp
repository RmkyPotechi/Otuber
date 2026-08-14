#include "core/mesh/deformer_graph.hpp"
#include "core/mesh/mesh.hpp"

#include <iostream>

int main()
{
    using namespace otuber::mesh;

    std::cout << "Otuber Preview\n";
    std::cout << "GUI/camera backend milestone scaffold\n";
    std::cout << "Core mesh + deformer graph is ready for the platform renderer.\n";

    const Mesh mesh = make_grid(16, 16);
    DeformerGraph graph;
    std::cout << "Preview mesh: " << mesh.vertices.size() << " vertices, "
              << mesh.triangles.size() << " triangles\n";

    return 0;
}
