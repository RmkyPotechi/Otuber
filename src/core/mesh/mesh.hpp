#pragma once

#include <cstdint>
#include <vector>

namespace otuber::mesh {

struct Vertex {
    float x = 0.0f;
    float y = 0.0f;
    float u = 0.0f;
    float v = 0.0f;
};

struct Triangle {
    std::uint32_t a = 0;
    std::uint32_t b = 0;
    std::uint32_t c = 0;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
};

Mesh make_grid(std::uint32_t columns, std::uint32_t rows);

} // namespace otuber::mesh
