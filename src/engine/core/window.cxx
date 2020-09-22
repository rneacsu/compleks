#include "window.hxx"

#include <stdexcept>

namespace compleks {

window::window()
{
    glfwWindowHint(GLFW_SAMPLES, 4);

    ctx = glfwCreateWindow(
        DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_TITLE, NULL, NULL);
    if (!ctx) {
        throw std::runtime_error("Could not create window");
    }

    glfwMakeContextCurrent(ctx);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw std::runtime_error((char *)glewGetErrorString(err));
    }

    en.enable_gl_logs();

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(ctx, (mode->width - DEFAULT_WIDTH) / 2,
        (mode->height - DEFAULT_HEIGHT) / 2);
}

void window::config(char const *title, int w, int h)
{
    glfwSetWindowTitle(ctx, title);
    glfwSetWindowSize(ctx, w, h);
}

void window::set_icon(const image &icon)
{
    GLFWimage img;
    img.width = icon.width;
    img.height = icon.height;
    img.pixels = icon.pixels;
    glfwSetWindowIcon(ctx, 1, &img);
}

window::~window()
{
    glfwDestroyWindow(ctx);
}

void window::run()
{
    glfwMakeContextCurrent(ctx);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(ctx)) {
        double time = glfwGetTime();

        render(time - old_time);
        old_time = time;

        glfwSwapBuffers(ctx);
        glfwPollEvents();
    }
}

void window::toggle_fullscreen()
{
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    if (!fullscreen) {
        glfwGetWindowSize(ctx, &width, &height);
        glfwGetWindowPos(ctx, &x, &y);
        glfwSetWindowMonitor(
            ctx, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowMonitor(ctx, NULL, x, y, width, height, 0);
    }

    fullscreen = !fullscreen;
}

void window::render(double)
{
}

}
