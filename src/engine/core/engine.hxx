#ifndef COMPLEKS_CORE_ENGINE_HXX
#define COMPLEKS_CORE_ENGINE_HXX

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../render/mesh.hxx"
#include "../utils/library.hxx"

namespace compleks {

class engine {
public:
    library<mesh> mesh_library;

    engine();
    ~engine();

    void enable_gl_logs();

private:
    static void glfw_error_func(int, const char *);
    static void gl_log_func(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar *message,
        const void *userParam);
};

}

#endif
