#include "auto_parts.hpp"

namespace otuber::avatar {

PartSet AutoPartAnalyzer::analyze(const otuber::texture::Image &image) const
{
    PartSet result;
    if (image.width == 0 || image.height == 0 || image.pixels.empty())
        return result;

    // First-pass segmentation: preserve the complete source as the base part.
    // Semantic eye/hair/mouth segmentation will be supplied by the vision
    // backend later; keeping it as a separate stage avoids coupling the core
    // to a particular ML model.
    result.parts.push_back({
        "base", PartType::Base, 0, 0,
        static_cast<int>(image.width), static_cast<int>(image.height), 0.0f});

    return result;
}

} // namespace otuber::avatar
