#pragma once

#include "core/texture/texture.hpp"

#include <string>
#include <vector>

namespace otuber::avatar {

enum class PartType { Base, Hair, Eye, Mouth, Other };

struct Part {
    std::string id;
    PartType type = PartType::Other;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    float depth = 0.0f;
};

struct PartSet {
    std::vector<Part> parts;
};

class AutoPartAnalyzer {
public:
    PartSet analyze(const otuber::texture::Image &image) const;
};

} // namespace otuber::avatar
