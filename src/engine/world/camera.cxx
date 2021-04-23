#include "camera.hxx"

#include <string>

#include <glm/gtx/rotate_vector.hpp>

#include "../core/engine.hxx"

namespace compleks {

camera::camera()
    : body("", MASS, false)
{
    body.create_body(std::make_unique<shape>(
        std::make_unique<btCapsuleShape>(RADIUS, HEIGHT - 2 * RADIUS)));

    body.rigid_body->setActivationState(DISABLE_DEACTIVATION);

    walk_front = walk_right = elevate = tilt = 0;
    sprint = false;
    cam_type = NORMAL;

    engine::get().add_listener(this);

    reset();
}

void camera::update(double delta)
{
    old_pos = pos;

    glm::vec3 velocity = body.get_velocity();
    glm::vec3 acc = glm::vec3(0);
    float top_speed = max_speed;

    if (cam_type == NORMAL) {
        velocity.y = 0;
    }

    if (cam_type == FREEFORM || sprint) {
        top_speed *= sprint_multiplier;
    }

    if (top_speed - glm::length(velocity) > 0
        && (walk_front || walk_right || (cam_type == FREEFORM && elevate))) {
        acc += glm::normalize(glm::vec3(front.x, 0, front.z))
            * (float)walk_front;
        acc += glm::normalize(glm::vec3(right.x, 0, right.z))
            * (float)walk_right;
        if (cam_type == FREEFORM) {
            acc += glm::vec3(0, 1, 0) * (float)elevate;
        }
        acc = glm::normalize(acc) * 40.0f;
    }

    acc += -velocity * 10.0f;
    acc *= body.get_mass();

    body.rigid_body->applyCentralForce(physics::to_bt(acc));
    body.update(delta);

    pos = body.pos + glm::vec3(0, EYE_HEIGHT - HEIGHT / 2, 0);

    if (tilt) {
        roll += tilt * (float)delta;
        mouse_move(0.0f, 0.0f);
    }
}

glm::mat4 camera::get_view_matrix(view_type t)
{
    glm::vec3 eye_pos = pos;
    switch (t) {
    case LEFT:
        eye_pos -= right * (eye_distance / 2);
        break;
    case RIGHT:
        eye_pos += right * (eye_distance / 2);
        break;
    }

    return glm::lookAt(eye_pos, eye_pos + front, up);
}

glm::mat4 camera::get_projection_matrix(int width, int height)
{
    return glm::perspective(fov, width / (float)height, NEAR_CLIP, FAR_CLIP);
}

glm::vec3 camera::get_position()
{
    return pos;
}

void camera::set_position(glm::vec3 p, glm::vec3 old)
{
    pos = p;
    old_pos = old;

    glm::vec3 delta = glm::vec3(0, EYE_HEIGHT - HEIGHT / 2, 0);

    body.set_position(pos - delta, old_pos - delta);
}

glm::vec3 camera::get_velocity()
{
    return body.get_velocity();
}

void camera::set_velocity(glm::vec3 v)
{
    body.set_velocity(v);
}

void camera::set_orientation(glm::vec3 euler)
{
    pitch = euler.x;
    yaw = euler.y;
    roll = euler.z;
    mouse_move(0.0f, 0.0f);
}

glm::vec3 camera::get_orientation()
{
    return glm::vec3(pitch, yaw, roll);
}

void camera::key_down(int key, int)
{
    switch (key) {
    case GLFW_KEY_W:
        walk_front += 1;
        break;
    case GLFW_KEY_S:
        walk_front -= 1;
        break;
    case GLFW_KEY_D:
        walk_right += 1;
        break;
    case GLFW_KEY_A:
        walk_right -= 1;
        break;
    case GLFW_KEY_E:
        tilt -= 1;
        break;
    case GLFW_KEY_Q:
        tilt += 1;
        break;
    case GLFW_KEY_SPACE:
        elevate += 1;
        break;
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:
        sprint = true;
        elevate -= 1;
        break;
    case GLFW_KEY_C:
        if (cam_type == NORMAL) {
            set_camera_type(FREEFORM);
        } else {
            set_camera_type(NORMAL);
        }
    }
}
void camera::key_up(int key, int)
{
    switch (key) {
    case GLFW_KEY_W:
        walk_front -= 1;
        break;
    case GLFW_KEY_S:
        walk_front += 1;
        break;
    case GLFW_KEY_D:
        walk_right -= 1;
        break;
    case GLFW_KEY_A:
        walk_right += 1;
        break;
    case GLFW_KEY_E:
        tilt += 1;
        break;
    case GLFW_KEY_Q:
        tilt -= 1;
        break;
    case GLFW_KEY_SPACE:
        elevate -= 1;
        break;
    case GLFW_KEY_LEFT_SHIFT:
    case GLFW_KEY_RIGHT_SHIFT:
        sprint = false;
        elevate += 1;
        break;
    }
}

void camera::mouse_move(float dx, float dy)
{
    if ((dx || dy) && !engine::get().focused) {
        return;
    }

    yaw += (dx * cos(roll) + dy * sin(roll)) * sensitivity;
    pitch += (dx * sin(roll) - dy * cos(roll)) * sensitivity;
    pitch = glm::clamp(pitch, glm::radians(-89.0f), glm::radians(89.0f));

    front = glm::normalize(glm::vec3(
        glm::cos(yaw) * glm::cos(pitch),
        glm::sin(pitch),
        glm::sin(yaw) * glm::cos(pitch)));
    right = glm::normalize(glm::vec3(
        -glm::sin(yaw) * glm::cos(roll),
        glm::sin(roll),
        glm::cos(yaw) * glm::cos(roll)));
    up = glm::cross(right, front);
}

void camera::mouse_scroll(float delta)
{
    if (!engine::get().focused) {
        return;
    }

    fov += -delta * 0.05f;
}

void camera::reset()
{
    pos = { 0, EYE_HEIGHT, 2.0f };
    up = { 0, 1, 0 };
    yaw = glm::radians(-90.0f);
    pitch = roll = 0;
    fov = glm::radians(60.0f);

    set_position(pos, pos);
    set_velocity({ 0, 0, 0 });
    set_camera_type(NORMAL);
    body.rigid_body->clearForces();

    mouse_move(0.0f, 0.0f);
}

glm::vec3 camera::get_old_position(void)
{
    return old_pos;
}

glm::vec3 camera::get_front(void)
{
    return front;
}

camera::camera_type camera::get_camera_type(void)
{
    return cam_type;
}

void camera::set_camera_type(camera::camera_type type)
{
    cam_type = type;
    body.set_gravity(type == NORMAL);
    body.set_collisions(type == NORMAL);
}

void camera::jump(void)
{
    glm::vec3 f = glm::vec3(0, JUMP_FORCE, 0) * body.get_mass();
    body.rigid_body->applyCentralImpulse(physics::to_bt(f));
}

} // namespace compleks
