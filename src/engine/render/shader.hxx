#ifndef __COMPLEKS_RENDER_SHADER_HXX
#define __COMPLEKS_RENDER_SHADER_HXX

#include <memory>

#include <GL/glew.h>

#include "../utils/resource.hxx"

namespace compleks
{

class shader {
public:
    enum type {
        VERTEX,
        FRAGMENT
    };

    GLuint id;

    shader(type type);
    ~shader();

    bool compile(std::unique_ptr<resource> src = nullptr);

private:
    std::unique_ptr<resource> src;
};

} // namespace compleks


#endif // __COMPLEKS_RENDER_SHADER_HXX