#include "scene6.hxx"

#include "../shapes/shapes.hxx"

scene6::scene6()
{
    sun = std::make_shared<compleks::lighting::light>();
    sun->pos = glm::vec3(40, 50, 40) * 20.0f;
    sun->ambient = glm::vec3(1) * 0.7f;
    sun->diffuse = glm::vec3(0);
    // sun->cut_off = glm::radians(10.0f);
    sun->direction = glm::normalize(-sun->pos);
    world.camera.set_position({ 5, 1.7, 5 }, { 5, 1.7, 5 });
    compleks::engine::get_lighting().add_light(sun);

    glClearColor(135 / 255.0f, 206 / 255.0f, 235 / 255.0f, 1.0f);

    plane = std::make_shared<compleks::body>("quad");
    plane->pos = { 0, 0, 0 };
    plane->scale = { 50, 50, 1 };
    plane->quat = glm::quat({ glm::radians(-90.0f), 0.0f, 0.0f });
    plane->color = { 151 / 255.0f, 255 / 255.0f, 99 / 255.0f, 1 };
    plane->create_body(std::make_unique<plane_shape>());

    gate1 = std::make_shared<compleks::body>("gate");
    gate1->pos = { -2, 0, 0 };
    gate1->scale = { 1.5, 1.5, 10 };
    gate1->color = { 1, 0.2, 0.2, 1 };
    gate1->quat = glm::quat({ 0.0f, glm::radians(90.0f), 0.0f });
    gate1->create_body(std::make_unique<gate_shape>());

    gate2 = std::make_shared<compleks::body>("gate");
    gate2->pos = { 0, 0, -2.375 };
    gate2->scale = { 1.5, 1.5, 10 };
    gate2->color = { 0.2, 0.2, 1, 1 };
    gate2->create_body(std::make_unique<gate_shape>());

    gate3 = std::make_shared<compleks::body>("gate");
    gate3->pos = { 0, 0, 2.375 };
    gate3->scale = { 1.5, 1.5, 10 };
    gate3->color = { 0.2, 1, 0.2, 1 };
    gate3->create_body(std::make_unique<gate_shape>());

    gate4 = std::make_shared<compleks::body>("gate");
    gate4->pos = { 2, 0, 0 };
    gate4->scale = { 1.5, 1.5, 10 };
    gate4->color = { 1, 1, 0.2, 1 };
    gate4->quat = glm::quat({ 0.0f, glm::radians(90.0f), 0.0f });
    gate4->create_body(std::make_unique<gate_shape>());

    world.static_objects.push_back(plane);
    world.static_objects.push_back(gate1);
    world.static_objects.push_back(gate2);
    world.static_objects.push_back(gate3);
    world.static_objects.push_back(gate4);

    p1 = std::make_shared<compleks::portal>();
    p2 = std::make_shared<compleks::portal>();
    p3 = std::make_shared<compleks::portal>();
    p4 = std::make_shared<compleks::portal>();

    p1->pos = { -3.25, 1.5, 0 };
    p1->scale = { 1.5, 3, 1 };
    p1->quat = glm::quat({ 0.0f, glm::radians(90.0f), 0.0f });

    p2->pos = { 3.25, 1.5, 0 };
    p2->scale = { 1.5, 3, 1 };
    p2->quat = glm::quat({ 0.0f, glm::radians(-90.0f), 0.0f });

    p3->pos = { 0, 1.5, -3.25 };
    p3->scale = { 1.5, 3, 1 };

    p4->pos = { 0, 1.5, 3.25 };
    p4->scale = { 1.5, 3, 1 };
    p4->quat = glm::quat({ 0.0f, glm::radians(180.0f), 0.0f });

    p1->set_target(p1);
    p2->set_target(p2);
    p3->set_target(p3);
    p4->set_target(p4);

    world.portals.push_back(p1);
    world.portals.push_back(p2);
    world.portals.push_back(p3);
    world.portals.push_back(p4);

    p1->sub_portals.push_back(p2);
    p2->sub_portals.push_back(p1);
    p3->sub_portals.push_back(p4);
    p4->sub_portals.push_back(p3);
}
