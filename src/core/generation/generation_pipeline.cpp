#include "generation_pipeline.hpp"

namespace otuber::generation {

GeneratedAssetSet GenerationPipeline::analyze_and_prepare(const GenerationRequest &request) const
{
    // The first implementation intentionally does not perform image generation.
    // It establishes the pipeline boundary so CV, inpainting, and expression
    // generators can be added without coupling them to the avatar runtime.
    GeneratedAssetSet result;

    if (request.generate_hidden_regions) {
        result.assets.push_back({"hidden_regions", GeneratedAssetType::HiddenRegion, "", "", 0.0f});
    }

    if (request.generate_eyelids) {
        result.assets.push_back({"eyelid_open", GeneratedAssetType::EyelidOpen, "", "", 0.0f});
        result.assets.push_back({"eyelid_half", GeneratedAssetType::EyelidHalf, "", "", 0.0f});
        result.assets.push_back({"eyelid_closed", GeneratedAssetType::EyelidClosed, "", "", 0.0f});
    }

    if (request.generate_expressions) {
        result.assets.push_back({"expressions", GeneratedAssetType::Expression, "", "", 0.0f});
    }

    return result;
}

} // namespace otuber::generation
