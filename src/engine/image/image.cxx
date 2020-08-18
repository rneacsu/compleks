#include "image.hxx"

#include <iostream>
#include <stdexcept>
#include <string>

#include "../utils/logger.hxx"

#include <Windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace compleks {

image::image(const resource &res)
{
    int n;

    logger::info("Loading image from resource");

    pixels = stbi_load_from_memory(
        (stbi_uc *)res.ptr, (int)res.size, &width, &height, &n, 4);

    if (!pixels) {
        throw std::runtime_error("Could not load image.");
    }
}

image::~image()
{
    logger::info("Releasing image");
    stbi_image_free(pixels);
}

} // namespace compleks
