#ifndef __COMPLEKS_RENDER_TEXTURE_HXX
#define __COMPLEKS_RENDER_TEXTURE_HXX

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../image/image.hxx"

namespace compleks {

class texture {
public:
    enum mode { NEAREST, LINEAR };

    texture(GLenum type);
    texture(const image &img, mode m = mode::LINEAR);
    ~texture();

    void bind();

private:
    GLenum type;
    GLuint id;
};

} // namespace compleks

#endif // __COMPLEKS_RENDER_TEXTURE_HXX
