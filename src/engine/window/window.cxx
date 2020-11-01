#include "window.hxx"

#include <stdexcept>
#include <string>

#include "../utils/logger.hxx"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace compleks {

int window::instances = 0;
std::mutex window::lock;

window::window()
{
    {
        std::lock_guard<std::mutex> guard(lock);

        if (instances++ == 0) {
            logger::info("Initializing window library");
            if (!glfwInit()) {
                throw std::runtime_error("Could not initialize window library");
            }

            glfwSetErrorCallback(glfw_error_func);
        }
    }

    glfwWindowHint(GLFW_SAMPLES, 4);

    ctx = glfwCreateWindow(
        DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_TITLE, NULL, NULL);
    if (!ctx) {
        throw std::runtime_error("Could not create window");
    }

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(ctx, (mode->width - DEFAULT_WIDTH) / 2,
        (mode->height - DEFAULT_HEIGHT) / 2);
}

void window::glfw_error_func(int code, const char *msg)
{
    logger::error(
        std::string("GLFW error: ") + msg + " (" + std::to_string(code) + ")");
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

void window::set_icon(int res_id)
{
    HWND win_h = glfwGetWin32Window(ctx);
    HICON icon_h = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(res_id));
    SendMessage(win_h, WM_SETICON, ICON_SMALL, (LPARAM)icon_h);
    SendMessage(win_h, WM_SETICON, ICON_BIG, (LPARAM)icon_h);
}

window::~window()
{
    glfwDestroyWindow(ctx);

    {
        std::lock_guard<std::mutex> guard(lock);

        if (--instances == 0) {
            logger::info("Closing window library");
            glfwTerminate();
        }
    }
}

void window::run()
{
    while (!glfwWindowShouldClose(ctx)) {
        glfwGetWindowSize(ctx, &width, &height);
        glfwGetWindowPos(ctx, &x, &y);

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

    static int old_w, old_h, old_x, old_y;

    if (!fullscreen) {
        old_w = width;
        old_h = height;
        old_x = x;
        old_y = y;
        glfwSetWindowMonitor(
            ctx, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowMonitor(ctx, NULL, old_x, old_y, old_w, old_h, 0);
    }

    fullscreen = !fullscreen;
}

void window::render(double)
{
}

}
