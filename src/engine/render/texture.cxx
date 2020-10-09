#include "texture.hxx"

#include <glm/gtc/type_ptr.hpp>

#include "../utils/logger.hxx"

namespace compleks {

texture::texture(const image &img)
{
    logger::info("Creating image texture");
    glGenTextures(1, &id);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, img.pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
}

texture::texture(glm::vec3 color)
{
    logger::info("Creating color texture ");
    glGenTextures(1, &id);
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGB, GL_FLOAT,
        glm::value_ptr(color));
}

texture::~texture()
{
    logger::info("Deleting texture");
    glDeleteTextures(1, &id);
}

void texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, id);
}

} // namespace compleks
