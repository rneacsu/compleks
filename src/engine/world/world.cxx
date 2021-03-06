#include "world.hxx"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "../core/engine.hxx"
#include "../utils/logger.hxx"

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
        engine::get_program().update_shaders();
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
    case GLFW_KEY_V:
        vr = !vr;
        if (!fullscreen) {
            if (vr) {
                glfwSetWindowSize(ctx, width * 2, height);
                glfwSetWindowPos(ctx, x - width / 2, y);
            } else {
                glfwSetWindowSize(ctx, width / 2, height);
                glfwSetWindowPos(ctx, x + width / 4, y);
            }
        }
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
    engine::get_meshes().add(id, path);
}

void world::render(double delta)
{
    int w, h;

    glfwGetFramebufferSize(ctx, &w, &h);
    if (w == 0 || h == 0) {
        return;
    }

    if (!cursor_enabled) {
        engine::get_physics().update(delta);
    }

    camera.update(delta);

    for (auto &obj : objects) {
        obj->update_all(delta);
    }

    for (auto &p : portals) {
        if (p->pass(camera)) {
            break;
        }
    }

    if (current_scene) {
        current_scene->update(delta);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, w, h);

    glm::mat4 view, proj;
    view = camera.get_view_matrix();
    proj = camera.get_projection_matrix(w, h);

    if (!vr) {
        render_portals(view, proj);
    } else {
        render_vr(w, h);
    }
}

void world::render_objects(glm::mat4 view, glm::mat4 proj)
{
    glm::vec3 eye_pos = glm::inverse(view) * glm::vec4(glm::vec3(0), 1);
    engine::get_lighting().update(eye_pos);

    set_transform(view, proj);

    for (auto &obj : objects) {
        obj->render();
    }
}

void world::render_portals(glm::mat4 view, glm::mat4 proj, int depth,
    std::shared_ptr<portal> current_portal)
{
    std::vector<std::weak_ptr<portal>> visible_portals;

    if (depth > 0 && current_portal) {
        visible_portals = current_portal->get_sub_portals();
    } else if (depth == 0) {
        visible_portals.insert(
            visible_portals.begin(), portals.begin(), portals.end());
    }

    glm::vec4 eye = glm::inverse(view) * glm::vec4(0, 0, 0, 1);

    glEnable(GL_STENCIL_TEST);

    for (auto &p_weak : visible_portals) {
        auto p = p_weak.lock();

        // Do not draw self
        if (p == current_portal) {
            continue;
        }

        // Skip invisible portals
        glm::vec3 normal = p->quat * glm::vec3(0, 0, 1);
        if (glm::dot(normal, glm::vec3(eye) - p->pos) < 0) {
            continue;
        }

        // Draw portal on stencil and increment
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        glDepthMask(GL_FALSE);

        glStencilFunc(GL_EQUAL, depth, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
        glStencilMask(0xFF);

        set_transform(view, proj);
        p->render();

        // Compute portal "camera" matrices
        glm::mat4 new_view = p->modify_view_matrix(view);
        glm::mat4 new_proj = p->modify_proj_matrix(view, proj);

        // If maximum recursive depth is reached, draw scene normaly
        if (depth == max_portal_depth) {
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

            glDepthMask(GL_TRUE);

            glStencilFunc(GL_EQUAL, depth + 1, 0xFF);
            glStencilMask(0x00);

            render_objects(new_view, new_proj);
        } else {
            // Render portals recursively
            render_portals(new_view, new_proj, depth + 1, p->get_target());
        }

        // Draw portal on stencil and decrement
        // Also overwrite the depth buffer to avoid drawing objects behind
        // the portal
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        glDepthFunc(GL_ALWAYS);
        glDepthMask(GL_TRUE);

        glStencilFunc(GL_EQUAL, depth + 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
        glStencilMask(0xFF);

        set_transform(view, proj);
        p->render();

        glDepthFunc(GL_LESS);
    }

    // Draw the rest of the scene
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glDepthMask(GL_TRUE);

    glStencilFunc(GL_EQUAL, depth, 0xFF);
    glStencilMask(0x00);

    render_objects(view, proj);
}

void world::render_vr(int w, int h)
{
    glm::mat4 proj, view;
    proj = camera.get_projection_matrix(w / 2, h);

    glViewport(0, 0, w / 2, h);
    view = camera.get_view_matrix(camera::view_type::LEFT);
    render_portals(view, proj);

    glViewport(w / 2, 0, w / 2, h);
    view = camera.get_view_matrix(camera::view_type::RIGHT);
    render_portals(view, proj);
}

void world::set_transform(glm::mat4 view, glm::mat4 proj)
{
    engine::get_program().set("projection_view_matrix", proj * view);
}

}
