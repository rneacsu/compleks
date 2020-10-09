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

    load_mesh("quad", "res/quad.obj");
    add_listener(&camera);
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
            camera.set_enabled(false);
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
            camera.set_enabled(true);
        }
    }
}

world::~world()
{
}

void world::load_mesh(std::string id, std::string path)
{
    en.mesh_library.add(id, path);
}

void world::render(double delta)
{
    int w, h;

    glfwGetFramebufferSize(ctx, &w, &h);
    if (w == 0 || h == 0) {
        return;
    }

    glViewport(0, 0, w, h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.update(delta);

    for (auto &obj : objects) {
        obj->update_all(delta);
    }

    for (auto &p : portals) {
        if (p->pass(camera)) {
            break;
        }
    }

    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 proj = camera.get_projection_matrix(w, h);

    render_portals(view, proj);
}

void world::render_objects(glm::mat4 view, glm::mat4 proj)
{
    glm::vec3 eye_pos = glm::inverse(view) * glm::vec4(glm::vec3(0), 1);
    lighting.update(program, eye_pos);

    program.set("projection_view_matrix", proj * view);

    for (auto &obj : objects) {
        obj->render(en, program);
    }
}

void world::render_portals(glm::mat4 view, glm::mat4 proj, int depth,
    std::shared_ptr<portal> current_portal)
{
    for (auto &p : portals) {
        if (p == current_portal) {
            continue;
        }

        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, depth, 0xFF);
        glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);
        glStencilMask(0xFF);

        program.set("projection_view_matrix", proj * view);
        p->render(en, program);

        glm::mat4 new_view = p->modify_view_matrix(view);
        glm::mat4 new_proj = p->modify_proj_matrix(view, proj);

        if (depth == max_portal_depth) {
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            glClear(GL_DEPTH_BUFFER_BIT);

            glEnable(GL_STENCIL_TEST);
            glStencilFunc(GL_EQUAL, depth + 1, 0xFF);
            glStencilMask(0x00);

            render_objects(new_view, new_proj);
        } else {
            render_portals(new_view, new_proj, depth + 1, p->get_target());
        }

        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glStencilFunc(GL_NOTEQUAL, depth + 1, 0xFF);
        glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);

        program.set("projection_view_matrix", proj * view);
        p->render(en, program);
    }

    glDisable(GL_STENCIL_TEST);
    glStencilMask(0x00);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_ALWAYS);

    glClear(GL_DEPTH_BUFFER_BIT);
    program.set("projection_view_matrix", proj * view);
    for (auto &p : portals)
        p->render(en, program);

    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x00);
    glStencilFunc(GL_LEQUAL, depth, 0xFF);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    render_objects(view, proj);
}

}
