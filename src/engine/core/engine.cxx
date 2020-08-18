#include "engine.hxx"

#include <stdexcept>
#include <string>

#include "../utils/logger.hxx"

using namespace std::literals;

namespace compleks {

engine::engine()
{
    if (!glfwInit()) {
        throw std::runtime_error("Could not initialize engine");
    }

    glfwSetErrorCallback(glfw_error_func);
}

engine::~engine()
{
    glfwTerminate();
}

void engine::enable_logs()
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_log_func, NULL);
}

void engine::glfw_error_func(int code, const char *msg)
{
    logger::error("GLFW error: "s + msg + " (" + std::to_string(code) + ")");
}

void engine::gl_log_func(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar *message, const void *) 
{
    std::string log = "source " + std::to_string(source)
            + " type " + std::to_string(type)
            + " id " + std::to_string(id)
            + " severity " + std::to_string(severity)
            + ":\n" + (length < 0 ? message : std::string(message, length));

    if (type == GL_DEBUG_TYPE_ERROR) {
        logger::error("GL error: " + log);
    } else {
        logger::info("GL log: " + log);
    }
}

}