#include "scene.hxx"

#include "../core/engine.hxx"

namespace compleks {

scene::scene()
    : world(engine::get_world())
{
    engine::get().add_listener(this);
}

scene::~scene()
{
    world.static_objects.clear();
    world.dynamic_objects.clear();
    world.portals.clear();
    world.camera.reset();
    engine::get_lighting().clear();
    engine::get().remove_listener(this);
}

std::shared_ptr<body> scene::object_ray_test()
{
    glm::vec3 pos = world.camera.get_position();
    glm::vec3 dir = world.camera.get_front();
    glm::vec3 end = pos + dir * 2.0f;

    auto obj = engine::get_physics().ray_test(pos, end, world.dynamic_objects);

    if (obj) {
        return obj;
    }

    auto closest_portal = world.get_closest_portal(pos);
    if (closest_portal && closest_portal->pass(pos, end)) {
        pos = closest_portal->pass(pos);
        end = closest_portal->pass(end);

        return engine::get_physics().ray_test(pos, end, world.dynamic_objects);
    }

    return nullptr;
}

void scene::object_mouse_down(body &, int)
{
}

void scene::mouse_down(int button)
{
    if (!engine::get().focused) {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (dragged_object) {
            dragged_object->rigid_body->setActivationState(ACTIVE_TAG);
            dragged_object->set_gravity(true);
            dragged_object = nullptr;
        } else {
            auto obj = object_ray_test();

            if (!obj) {
                return;
            }

            obj->rigid_body->setActivationState(DISABLE_DEACTIVATION);
            obj->set_gravity(false);
            dragged_object = obj;

            object_mouse_down(*obj, button);
        }
    }
}

void scene::key_down(int key, int)
{
    if (!engine::get().focused) {
        return;
    }

    if (key == GLFW_KEY_SPACE
        && world.camera.get_camera_type() == camera::camera_type::NORMAL) {
        glm::vec3 start = world.camera.get_position();
        glm::vec3 end
            = start + glm::vec3(0, -1.05, 0) * world.camera.EYE_HEIGHT;

        if (engine::get_physics().ray_test(start, end)) {
            world.camera.jump();
        }
    }
}

void scene::update(double)
{
    if (dragged_object) {
        glm::vec3 eye_pos = world.camera.get_position();
        glm::vec3 target_pos
            = eye_pos + world.camera.get_front() * GRAB_DISTANCE;
        glm::vec3 current_pos = dragged_object->pos;
        glm::vec3 clone_target_pos, tmp;

        clone_target_pos = target_pos;

        auto p = dragged_object->clone_portal;
        if (p) {
            auto t = p->get_target();
            if (t) {
                if (glm::length(target_pos - p->pos)
                    > glm::length(target_pos - t->pos)) {
                    p = t;
                }
            }
            clone_target_pos = p->pass(target_pos);
        } else {
            auto closest_portal = world.get_closest_portal(eye_pos);

            if (closest_portal) {
                clone_target_pos = closest_portal->pass(target_pos);
            }
        }

        if (glm::length(target_pos - current_pos)
            > glm::length(clone_target_pos - current_pos)) {
            tmp = target_pos;
            target_pos = clone_target_pos;
            clone_target_pos = tmp;
        }

        apply_drag_force(*dragged_object, target_pos);

        if (p) {
            apply_drag_force(*dragged_object->clone_body, clone_target_pos);
        }
    }
}

void scene::apply_drag_force(body &b, glm::vec3 target_pos)
{
    glm::vec3 dir = target_pos - b.pos;
    float len = glm::clamp(glm::length(dir), 0.0f, GRAB_DISTANCE);
    glm::vec3 force
        = glm::normalize(dir) * len * dragged_object->get_mass() * GRAB_FORCE;

    b.set_velocity(glm::vec3());
    b.rigid_body->setAngularVelocity(btVector3(0, 0, 0));
    b.rigid_body->applyCentralForce(physics::to_bt(force));
}

void scene::load_scene(std::string path)
{
    resource r(path);
    config = nlohmann::ordered_json::parse(std::string(r.ptr, r.size));
    scene_loader(*this).load(config);
}

} // namespace compleks
