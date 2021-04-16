#include "input_window.hxx"

#include <iostream>
#include <string>

namespace compleks {

std::map<GLFWwindow *, std::pair<input_window *, std::list<input_listener *>>>
    input_window::listeners;

input_window::input_window()
{
    glfwSetKeyCallback(ctx, key_callback);
    glfwSetCursorPosCallback(ctx, mouse_move_callback);
    glfwSetMouseButtonCallback(ctx, mouse_button_callback);
    glfwSetCursorEnterCallback(ctx, mouse_enter_callback);
    glfwSetScrollCallback(ctx, mouse_scroll_callback);
    glfwSetFramebufferSizeCallback(ctx, window_resize_callback);

    add_listener(this);
}

void input_window::add_listener(input_listener *l)
{
    if (listeners.find(ctx) == listeners.end()) {
        listeners[ctx] = std::make_pair(this, std::list<input_listener *>());
    }
    listeners[ctx].second.push_back(l);
}

void input_window::remove_listener(input_listener *l)
{
    listeners[ctx].second.remove(l);
}

void input_window::key_callback(
    GLFWwindow *ctx,
    int key,
    int,
    int action,
    int mods)
{
    for (auto &listener : listeners[ctx].second) {
        if (action == GLFW_PRESS) {
            listener->key_down(key, mods);
        } else if (action == GLFW_RELEASE) {
            listener->key_up(key, mods);
        }
    }
}

void input_window::mouse_move_callback(GLFWwindow *ctx, double x, double y)
{
    auto &instance = listeners[ctx];
    input_window *window = instance.first;
    auto &window_listeners = instance.second;

    float dx = (float)(x - window->old_x);
    float dy = (float)(y - window->old_y);

    window->old_x = x;
    window->old_y = y;

    for (auto &listener : window_listeners) {
        listener->mouse_move(dx, dy);
    }
}

void input_window::mouse_button_callback(
    GLFWwindow *ctx,
    int button,
    int action,
    int)
{
    for (auto &listener : listeners[ctx].second) {
        if (action == GLFW_PRESS) {
            listener->mouse_down(button);
        } else if (action == GLFW_RELEASE) {
            listener->mouse_up(button);
        }
    }
}

void input_window::mouse_enter_callback(GLFWwindow *ctx, int enter)
{
    auto &instance = listeners[ctx];
    input_window *window = instance.first;
    auto &window_listeners = instance.second;

    glfwGetCursorPos(ctx, &window->old_x, &window->old_y);

    for (auto &listener : window_listeners) {
        if (enter) {
            listener->mouse_enter();
        } else {
            listener->mouse_leave();
        }
    }
}

void input_window::mouse_scroll_callback(GLFWwindow *ctx, double, double dy)
{
    for (auto &listener : listeners[ctx].second) {
        listener->mouse_scroll((float)dy);
    }
}

void input_window::window_resize_callback(
    GLFWwindow *ctx,
    int width,
    int height)
{
    for (auto &listener : listeners[ctx].second) {
        listener->window_resize(width, height);
    }
}

void input_window::set_cursor(bool enabled)
{
    if (enabled) {
        glfwSetInputMode(ctx, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(ctx, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    focused = !enabled;
    glfwGetCursorPos(ctx, &old_x, &old_y);
}

}
