#include "mesh.hpp"

#include <algorithm>

namespace otuber::mesh {

Mesh make_grid(std::uint32_t columns, std::uint32_t rows)
{
    Mesh mesh;

    columns = std::max<std::uint32_t>(columns, 1);
    rows = std::max<std::uint32_t>(rows, 1);

    mesh.vertices.reserve((columns + 1) * (rows + 1));
    mesh.triangles.reserve(columns * rows * 2);

    for (std::uint32_t y = 0; y <= rows; ++y) {
        for (std::uint32_t x = 0; x <= columns; ++x) {
            const float fx = static_cast<float>(x) / static_cast<float>(columns);
            const float fy = static_cast<float>(y) / static_cast<float>(rows);
            mesh.vertices.push_back({fx * 2.0f - 1.0f, fy * 2.0f - 1.0f, fx, fy});
        }
    }

    const std::uint32_t stride = columns + 1;
    for (std::uint32_t y = 0; y < rows; ++y) {
        for (std::uint32_t x = 0; x < columns; ++x) {
            const std::uint32_t top_left = y * stride + x;
            const std::uint32_t top_right = top_left + 1;
            const std::uint32_t bottom_left = (y + 1) * stride + x;
            const std::uint32_t bottom_right = bottom_left + 1;

            mesh.triangles.push_back({top_left, bottom_left, top_right});
            mesh.triangles.push_back({top_right, bottom_left, bottom_right});
        }
    }

    return mesh;
}

} // namespace otuber::mesh
