#include "bayer.hxx"

namespace compleks {

const size_t bayer::size = 8;
unsigned char bayer::pattern[64] = { 0, 32, 8, 40, 2, 34, 10, 42, 48, 16, 56,
    24, 50, 18, 58, 26, 12, 44, 4, 36, 14, 46, 6, 38, 60, 28, 52, 20, 62, 30,
    54, 22, 3, 35, 11, 43, 1, 33, 9, 41, 51, 19, 59, 27, 49, 17, 57, 25, 15, 47,
    7, 39, 13, 45, 5, 37, 63, 31, 55, 23, 61, 29, 53, 21 };

bayer::bayer()
{
    width = height = size;
    pixels = new unsigned char[4 * width * height];

    for (size_t i = 0; i < width * height; i++) {
        pixels[4 * i] = pixels[4 * i + 1] = pixels[4 * i + 2] = pattern[i];
        pixels[4 * i + 3] = 255;
    }
}

bayer::~bayer()
{
    delete[] pixels;
    pixels = nullptr;
}

} // namespace compleks
