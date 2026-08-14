#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace otuber::texture {

struct Image {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint8_t channels = 4;
    std::vector<std::uint8_t> pixels;
};

// Runtime-neutral image container. Decoding is intentionally kept separate
// so OBS/GUI/platform backends can provide their preferred image loader.
class Texture {
public:
    bool set_image(Image image);
    bool empty() const { return image_.pixels.empty(); }
    const Image &image() const { return image_; }

private:
    Image image_;
};

} // namespace otuber::texture
