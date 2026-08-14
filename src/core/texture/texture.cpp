#include "texture.hpp"

namespace otuber::texture {

bool Texture::set_image(Image image)
{
    if (image.width == 0 || image.height == 0 || image.channels == 0)
        return false;

    const std::size_t expected = static_cast<std::size_t>(image.width) *
                                 static_cast<std::size_t>(image.height) *
                                 image.channels;
    if (image.pixels.size() != expected)
        return false;

    image_ = std::move(image);
    return true;
}

} // namespace otuber::texture
