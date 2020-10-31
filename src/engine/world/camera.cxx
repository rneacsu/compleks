#include "camera.hxx"

#include <iostream>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/gtx/rotate_vector.hpp>

#include "../utils/logger.hxx"

using namespace std::literals;

namespace compleks {

camera::camera()
{
    reset();
}

void camera::update(double delta)
{
    if (!enabled) {
        return;
    }

    old_pos = pos;

    if (walk_front || walk_right || elevate) {
        acceleration
            = glm::normalize(glm::vec3(front.x, 0, front.z)) * (float)walk_front
            + glm::normalize(glm::vec3(right.x, 0, right.z)) * (float)walk_right
            + glm::vec3(0, 1, 0) * (float)elevate;
        acceleration = glm::normalize(acceleration)
            * (sprint ? sprint_multiplier : 1.0f);
    } else {
        acceleration = glm::vec3();
    }

    velocity
        += (acceleration * drag_factor * max_speed - velocity * drag_factor)
        * (float)delta;
    pos += velocity * (float)delta;

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
    return glm::perspective(fov, width / (float)height, 0.001f, 100.0f);
}

glm::vec3 camera::get_position()
{
    return pos;
}

void camera::set_position(glm::vec3 p)
{
    pos = old_pos = p;
}

glm::vec3 camera::get_velocity()
{
    return velocity;
}

void camera::set_velocity(glm::vec3 v)
{
    velocity = v;
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

void camera::set_enabled(bool en)
{
    enabled = en;
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
        elevate -= 1;
        break;
    case GLFW_KEY_RIGHT_SHIFT:
        sprint = true;
        break;
    case GLFW_KEY_R:
        reset();
        break;
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
        elevate += 1;
        break;
    case GLFW_KEY_RIGHT_SHIFT:
        sprint = false;
        break;
    }
}

void camera::mouse_move(float dx, float dy)
{
    if ((dx || dy) && !enabled) {
        return;
    }

    yaw += (dx * cos(roll) + dy * sin(roll)) * sensitivity;
    pitch += (dx * sin(roll) - dy * cos(roll)) * sensitivity;
    pitch
        = glm::min(glm::max(pitch, glm::radians(-89.0f)), glm::radians(89.0f));

    front = glm::normalize(glm::vec3(glm::cos(yaw) * glm::cos(pitch),
        glm::sin(pitch), glm::sin(yaw) * glm::cos(pitch)));
    right = glm::normalize(glm::vec3(-glm::sin(yaw) * glm::cos(roll),
        glm::sin(roll), glm::cos(yaw) * glm::cos(roll)));
    up = glm::cross(right, front);
}

void camera::mouse_scroll(float delta)
{
    if (!enabled) {
        return;
    }

    fov += -delta * 0.05f;
}

void camera::reset()
{
    pos = { 1.5, 2, 1.5 };
    up = { 0, 1, 0 };
    yaw = glm::radians(-135.0f);
    pitch = glm::radians(-30.0f);
    roll = 0;
    fov = glm::radians(60.0f);
    walk_front = walk_right = elevate = tilt = 0;
    acceleration = velocity = glm::vec3();
    sprint = false;
    mouse_move(0.0f, 0.0f);
}

glm::vec3 camera::get_old_position(void)
{
    return old_pos;
}

} // namespace compleks
