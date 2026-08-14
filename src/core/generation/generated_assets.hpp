#pragma once

#include <string>
#include <vector>

namespace otuber::generation {

enum class GeneratedAssetType {
    HiddenRegion,
    EyelidOpen,
    EyelidHalf,
    EyelidClosed,
    Expression,
    OcclusionFill,
};

struct GeneratedAsset {
    std::string id;
    GeneratedAssetType type = GeneratedAssetType::OcclusionFill;
    std::string source_part_id;
    std::string texture_path;
    float confidence = 0.0f;
};

struct GeneratedAssetSet {
    std::vector<GeneratedAsset> assets;
};

} // namespace otuber::generation
