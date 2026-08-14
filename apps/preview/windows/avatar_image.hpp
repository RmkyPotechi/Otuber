#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace otuber::preview {

struct AvatarImage {
    int width = 0;
    int height = 0;
    std::vector<std::uint8_t> rgba;

    bool empty() const { return rgba.empty(); }
};

bool load_png_wic(const std::wstring &path, AvatarImage &image);

} // namespace otuber::preview
