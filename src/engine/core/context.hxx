#ifndef __COMPLEKS_CORE_CONTEXT_HXX
#define __COMPLEKS_CORE_CONTEXT_HXX

#include "../window/input_window.hxx"

namespace compleks {

class context : public input_window {
public:
    context();
    ~context();

private:
    static void gl_log_func(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar *message,
        const void *userParam);
};

} // namespace compleks

#endif // __COMPLEKS_CORE_CONTEXT_HXX
