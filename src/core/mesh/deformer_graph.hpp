#pragma once

#include "deformation.hpp"

#include <string>
#include <vector>

namespace otuber::mesh {

struct DeformerNode {
    std::string id;
    DeformationRegion region;
    RegionTransform transform;
    float weight = 1.0f;
    bool enabled = true;
};

class DeformerGraph {
public:
    void add(DeformerNode node);
    void clear();
    bool set_weight(const std::string &id, float weight);
    bool set_transform(const std::string &id, RegionTransform transform);
    Mesh evaluate(const Mesh &source) const;
    std::size_t size() const { return nodes_.size(); }

private:
    std::vector<DeformerNode> nodes_;
};

} // namespace otuber::mesh
