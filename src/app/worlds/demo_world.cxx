#include "demo_world.hxx"

#include <glm/gtc/constants.hpp>

demo_world::demo_world()
{
    sun.ambient = glm::vec3(1.0f, 1.0f, 1.0f) * 0.25f;
    sun.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    sun.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    sun.pos = glm::vec3(40, 50, 60);
    // sun.attenuation = glm::vec3(1, 0.01, 0.001);
    sun.attenuation = glm::vec3(1, 0, 0);
    sun.cut_off = glm::radians(10.0f);
    sun.direction = glm::normalize(-sun.pos);

    glClearColor(135 / 255.0f, 206 / 255.0f, 235 / 255.0f, 1.0f);

    lighting.add_light(&sun);

    compleks::mesh_library::add("cube", "res/cube.dae");
    compleks::mesh_library::add("square", "res/plane.obj");

    cube = compleks::object("cube");
    plane = compleks::object("square");

    cube.pos = { 0, 0.5, 0 };
    plane.scale = { 50, 50, 1 };
    plane.pitch = -glm::half_pi<float>();
    plane.pos = { 0, 0, 0 };

    objects.push_back(&cube);
    objects.push_back(&plane);

    camera.set_position({ 1, 1, 1 });
    camera.set_orientation(
        -3 * glm::quarter_pi<float>(), -glm::quarter_pi<float>() * 0.75f, 0);
}