#include "world.hxx"

#include <glm/gtc/type_ptr.hpp>

#include "../core/engine.hxx"
#include "../utils/logger.hxx"

namespace compleks {

world::world()
{
}

world::~world()
{
}

void world::update(double delta)
{
    engine::get_physics().update(delta);
    camera.update(delta);

    for (auto &p : portals) {
        if (p->pass(camera)) {
            break;
        }
    }

    for (auto &obj : dynamic_objects) {
        obj->update(delta);

        for (auto &p : portals) {
            if (p->pass(*obj)) {
                break;
            }
        }

        auto p = get_closest_portal(obj->pos);

        if (p && p->get_target() && p->contact(*obj)) {
            obj->create_clone(p);
        } else {
            obj->remove_clone();
        }
    }

    if (current_scene) {
        current_scene->update(delta);
    }
}

void world::render(double delta, int w, int h, bool vr)
{
    if (engine::get().focused) {
        update(delta);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    engine::get_program().set("fog", fog);

    glm::mat4 view, proj;
    if (!vr) {
        glViewport(0, 0, w, h);

        view = camera.get_view_matrix();
        proj = camera.get_projection_matrix(w, h);

        render_portals(view, proj);
    } else {
        proj = camera.get_projection_matrix(w / 2, h);

        glViewport(0, 0, w / 2, h);
        view = camera.get_view_matrix(camera::view_type::LEFT);
        render_portals(view, proj);

        glViewport(w / 2, 0, w / 2, h);
        view = camera.get_view_matrix(camera::view_type::RIGHT);
        render_portals(view, proj);
    }

    engine::get_lighting().disable();
    engine::get_program().set("fog", glm::vec2(0));

    proj = glm::ortho(0.0f, (float)w, 0.0f, (float)h, -1.0f, 1.0f);
    view = glm::scale(glm::mat4(1), glm::vec3(1.0f, 1.0f, 1.0f));

    glDepthFunc(GL_ALWAYS);
    set_transform(view, proj);
    for (auto &obj : hud) {
        obj->render();
    }
    glDepthFunc(GL_LESS);
}

void world::render_objects(glm::mat4 view, glm::mat4 proj)
{
    glm::vec3 eye_pos = glm::inverse(view) * glm::vec4(0, 0, 0, 1);

    engine::get_skybox().render(view, proj);
    engine::get_lighting().update(eye_pos);

    set_transform(view, proj);

    for (auto &obj : dynamic_objects) {
        if (obj->clone_body) {
            engine::get_program().set("clip", true);

            auto p = obj->clone_portal;

            engine::get_program().set("clip_plane", p->get_clip_plane());
            obj->render();

            auto t = p->get_target();
            if (!t) {
                continue;
            }

            engine::get_program().set("clip_plane", t->get_clip_plane());
            obj->clone_body->render();

            engine::get_program().set("clip", false);
        } else {
            obj->render();
        }
    }

    for (auto &obj : static_objects) {
        obj->render();
    }
}

void world::render_portals(
    glm::mat4 view,
    glm::mat4 proj,
    int depth,
    std::shared_ptr<portal> current_portal)
{
    std::vector<std::weak_ptr<portal>> visible_portals;

    if (depth > 0 && current_portal) {
        visible_portals = current_portal->sub_portals;
    } else if (depth == 0) {
        visible_portals.insert(
            visible_portals.begin(),
            portals.begin(),
            portals.end());
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
        if (glm::dot(p->get_normal(), glm::vec3(eye) - p->pos) < 0) {
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

void world::set_transform(glm::mat4 view, glm::mat4 proj)
{
    engine::get_program().set("projection_view_matrix", proj * view);
}

void world::set_fog(float start, float end)
{
    fog.x = start;
    fog.y = end;
}

std::shared_ptr<portal> world::get_closest_portal(glm::vec3 pos)
{
    float min_dist = 5;
    std::shared_ptr<portal> closest_portal;

    for (auto &p : portals) {
        float dist = glm::length(p->pos - pos);

        if (dist < min_dist) {
            glm::vec3 n = p->get_normal();

            if (glm::dot(n, pos - p->pos) > 0) {
                min_dist = dist;
                closest_portal = p;
            }
        }
    }

    return closest_portal;
}

}
