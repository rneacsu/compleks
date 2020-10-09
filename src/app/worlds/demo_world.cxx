#include "demo_world.hxx"

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
    lighting.add_light(&sun);

    glClearColor(135 / 255.0f, 206 / 255.0f, 235 / 255.0f, 1.0f);

    load_mesh("cube", "res/cube.obj");
    cube = std::make_shared<compleks::object>("cube");
    plane = std::make_shared<compleks::object>("quad");

    cube->pos = { 0, 0.5, 0 };
    plane->pos = { 0, 0, 0 };
    plane->scale = { 50, 50, 1 };
    plane->quat = glm::quat({ glm::radians(-90.0f), 0.0f, 0.0f });

    objects.push_back(cube);
    objects.push_back(plane);

    p1 = std::make_shared<compleks::portal>();
    p2 = std::make_shared<compleks::portal>();

    // p1->pos = { 2, 0.5, 0 };
    p1->pos = { 0, 0.5, -2 };
    p1->scale = { 2, 4, 1 };
    // p1->quat = glm::quat({ 0.0f, glm::radians(-90.0f), 0.0f });

    p2->pos = { 0, 0.5, 2 };
    p2->scale = { 2, 4, 1 };
    p2->quat = glm::quat({ 0.0f, glm::radians(180.0f), 0.0f });

    p1->set_target(p2);
    p2->set_target(p1);
    portals.push_back(p1);
    portals.push_back(p2);
}
