#include "deformer_graph.hpp"

namespace otuber::mesh {

void DeformerGraph::add(DeformerNode node)
{
    nodes_.push_back(std::move(node));
}

void DeformerGraph::clear()
{
    nodes_.clear();
}

Mesh DeformerGraph::evaluate(const Mesh &source) const
{
    Mesh result = source;

    for (const auto &node : nodes_) {
        if (!node.enabled || node.weight <= 0.0f)
            continue;

        RegionTransform transform = node.transform;
        transform.translate_x *= node.weight;
        transform.translate_y *= node.weight;
        transform.rotation *= node.weight;
        transform.scale_x = 1.0f + (transform.scale_x - 1.0f) * node.weight;
        transform.scale_y = 1.0f + (transform.scale_y - 1.0f) * node.weight;

        result = apply_region_transform(result, node.region, transform);
    }

    return result;
}

} // namespace otuber::mesh
