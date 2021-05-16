#include "image.hxx"

#include <iostream>
#include <stdexcept>
#include <string>

#include "../utils/logger.hxx"

#include <Windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace compleks {

image::image()
{
    pixels = nullptr;
    width = height = 0;
}

image::image(const resource &res)
{
    int n;

    logger::info("Creating image from resource");

    pixels = stbi_load_from_memory(
        (stbi_uc *)res.ptr,
        (int)res.size,
        &width,
        &height,
        &n,
        4);

    if (!pixels) {
        throw std::runtime_error("Could not load image.");
    }
}

image::image(image &&img)
{
    width = img.width;
    height = img.height;
    pixels = img.pixels;
    img.pixels = nullptr;
}

image::~image()
{
    if (pixels) {
        logger::info("Destroying image");
        stbi_image_free(pixels);
    }
}

} // namespace compleks
