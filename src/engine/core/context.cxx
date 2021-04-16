#include "context.hxx"

#include <stdexcept>

#include "../utils/logger.hxx"

namespace compleks {

context::context()
{
    logger::info("Creating OpenGL context");

    glfwMakeContextCurrent(ctx);
    glfwSwapInterval(1);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw std::runtime_error((char *)glewGetErrorString(err));
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_log_func, NULL);
    logger::info(
        "OpenGL version: "
        + std::string((const char *)glGetString(GL_VERSION)));
    logger::info("GPU: " + std::string((const char *)glGetString(GL_RENDERER)));
}

context::~context()
{
    logger::info("Destroying OpenGL context");
}

void context::gl_log_func(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }
    std::string log = "source " + std::to_string(source) + " type "
        + std::to_string(type) + " id " + std::to_string(id) + " severity "
        + std::to_string(severity) + ":\n"
        + (length < 0 ? message : std::string(message, length));

    if (type == GL_DEBUG_TYPE_ERROR) {
        logger::error("GL error: " + log);
    } else {
        logger::info("GL log: " + log);
    }
}

} // namespace compleks
