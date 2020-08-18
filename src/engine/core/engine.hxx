#ifndef COMPLEKS_CORE_ENGINE_HXX 
#define  COMPLEKS_CORE_ENGINE_HXX

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace compleks {

class engine {
private:
    engine();
    ~engine();

    friend class window;

    void enable_logs();

    static void glfw_error_func(int, const char *);
    static void gl_log_func(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam);
};

}

#endif