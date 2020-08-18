#include "camera.hxx"

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
    float multiplier = sprint ? 3.0f : 1.5f;

    delta *= multiplier;

    pos += glm::normalize(glm::vec3(front.x, 0, front.z)) * (float)delta
        * (float)walk_front;
    pos += glm::normalize(glm::vec3(right.x, 0, right.z)) * (float)delta
        * (float)walk_right;
    pos += glm::normalize(glm::vec3(0, 1, 0)) * (float)delta * (float)elevate;

    if (tilt) {
        roll += tilt * (float)delta;
        mouse_move(0.0f, 0.0f);
    }
}

glm::mat4 camera::get_view_matrix()
{
    return glm::lookAt(pos, pos + front, up);
}

glm::mat4 camera::get_projection_matrix(int width, int height)
{
    return glm::perspective(fov, width / (float)height, 0.1f, 100.0f);
}

glm::vec3 camera::get_position()
{
    return pos;
}

void camera::set_position(glm::vec3 p)
{
    pos = p;
}

void camera::set_orientation(float y, float p, float r)
{
    yaw = y;
    pitch = p;
    roll = r;
    mouse_move(0.0f, 0.0f);
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
    yaw += (dx * cos(roll) + dy * sin(roll)) * 0.005f;
    pitch += (dx * sin(roll) - dy * cos(roll)) * 0.005f;
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
    fov += -delta * 0.05f;
}

void camera::reset()
{
    pos = { 0, 0, 1 };
    up = { 0, 1, 0 };
    pitch = roll = 0;
    yaw = -glm::half_pi<float>();
    fov = glm::radians(60.0f);
    walk_front = walk_right = elevate = tilt = 0;
    sprint = false;
    mouse_move(0.0f, 0.0f);
}

} // namespace compleks
