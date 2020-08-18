#ifndef __COMPLEKS_RENDER_TEXTURE_HXX
#define __COMPLEKS_RENDER_TEXTURE_HXX

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../image/image.hxx"

namespace compleks {

class texture {
public:
    texture(const image &img);
    texture(glm::vec4 color);
    ~texture();

    void bind();

private:
    GLuint id;
};

} // namespace compleks

#endif // __COMPLEKS_RENDER_TEXTURE_HXX