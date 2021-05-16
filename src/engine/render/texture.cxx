#include "texture.hxx"

#include <glm/gtc/type_ptr.hpp>

#include "../utils/logger.hxx"

namespace compleks {

texture::texture(GLenum type)
    : type(type)
{
    logger::info("Creating texture");
    glGenTextures(1, &id);
    bind();
}

texture::texture(const image &img, mode m)
    : texture(GL_TEXTURE_2D)
{
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (m == mode::LINEAR) {
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    float aniso = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
    glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY, aniso);

    glTexImage2D(
        type,
        0,
        GL_RGBA,
        img.width,
        img.height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        img.pixels);
    glGenerateMipmap(type);
}

texture::~texture()
{
    logger::info("Destroying texture");
    glDeleteTextures(1, &id);
}

void texture::bind()
{
    glBindTexture(type, id);
}

} // namespace compleks
