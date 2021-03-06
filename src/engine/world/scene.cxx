#include "scene.hxx"

#include "../utils/logger.hxx"
#include "world.hxx"

namespace compleks {

scene::scene(compleks::world &world)
    : world(world)
{
}

scene::~scene()
{
    world.objects.clear();
    world.portals.clear();
    world.camera.reset();
    engine::get_lighting().clear();
}

body *scene::object_ray_test()
{
    glm::vec3 pos = world.camera.get_position();
    glm::vec3 dir = world.camera.get_front();
    glm::vec3 end = pos + dir * 2.0f;

    btVector3 ray_start(pos.x, pos.y, pos.z);
    btVector3 ray_end(end.x, end.y, end.z);
    btCollisionWorld::ClosestRayResultCallback ray_callback(ray_start, ray_end);

    engine::get_physics().get_dynamics_world().rayTest(
        ray_start, ray_end, ray_callback);

    if (ray_callback.hasHit()) {
        btRigidBody *rigid_body = (btRigidBody *)ray_callback.m_collisionObject;

        return (body *)rigid_body->getUserPointer();
    }

    return nullptr;
}

void scene::object_mouse_down(body &, int)
{
}

void scene::mouse_down(int button)
{
    if (world.cursor_enabled) {
        return;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {

        if (dragged_object) {
            dragged_object->rigid_body->setActivationState(ACTIVE_TAG);
            dragged_object = nullptr;
        } else {
            body *obj = object_ray_test();

            if (!obj) {
                return;
            }

            if (obj->mass) {
                obj->rigid_body->setActivationState(DISABLE_DEACTIVATION);
                dragged_object = obj;
            }

            object_mouse_down(*obj, button);
        }
    }
}

void scene::update(double)
{
    if (dragged_object) {
        glm::vec3 target_pos
            = world.camera.get_position() + world.camera.get_front() * 2.0f;
        glm::vec3 current_pos = dragged_object->pos;
        glm::vec3 force
            = (target_pos - current_pos) * dragged_object->mass * 300.0f;

        dragged_object->set_velocity(glm::vec3());
        dragged_object->rigid_body->setAngularVelocity(btVector3(0, 0, 0));
        dragged_object->rigid_body->applyCentralForce(
            btVector3(force.x, force.y, force.z));
    }
}

} // namespace compleks
