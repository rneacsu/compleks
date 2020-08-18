#include "world.hxx"

#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/logger.hxx"

using namespace std::literals;

namespace compleks {

world::world()
{
    glClearColor(0, 0, 0, 1);

    glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);
    glLineWidth(1);
    glPointSize(3);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void world::key_down(int key, int mods)
{
    input_window::key_down(key, mods);
    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (cursor_enabled) {
            glfwSetWindowShouldClose(ctx, true);
        } else {
            set_cursor(true);
            remove_listener(&camera);
        }
        break;
    case GLFW_KEY_F11:
        toggle_fullscreen();
        break;
    case GLFW_KEY_ENTER:
        if (mods & GLFW_MOD_ALT) {
            toggle_fullscreen();
        }
        break;
    case GLFW_KEY_F10:
        logger::info("Recompiling shaders...");
        program.update_shaders();
        break;
    case GLFW_KEY_F12:
        if (polygon_mode == GL_FILL) {
            glDisable(GL_CULL_FACE);
            polygon_mode = GL_LINE;
        } else if (polygon_mode == GL_LINE) {
            polygon_mode = GL_POINT;
        } else {
            glEnable(GL_CULL_FACE);
            polygon_mode = GL_FILL;
        }
        glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);

        break;
    }
}

void world::mouse_down(int button)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (cursor_enabled) {
            set_cursor(false);
            add_listener(&camera);
        }
    }
}

world::~world() { }

void world::render(double delta)
{
    int w, h;

    glfwGetFramebufferSize(ctx, &w, &h);
    glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.update(delta);
    lighting.update(program, camera.get_position());

    program.set("projection_view_matrix",
        camera.get_projection_matrix(w, h) * camera.get_view_matrix());

    for (auto& obj : objects) {
        obj->render(program, delta);
    }
}

}