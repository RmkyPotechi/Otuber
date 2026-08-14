#pragma once

#include "generated_assets.hpp"

#include <string>

namespace otuber::generation {

struct GenerationRequest {
    std::string source_image;
    bool generate_hidden_regions = true;
    bool generate_eyelids = true;
    bool generate_expressions = true;
};

class GenerationPipeline {
public:
    GeneratedAssetSet analyze_and_prepare(const GenerationRequest &request) const;
};

} // namespace otuber::generation
