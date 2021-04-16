#include "scene3.hxx"

#include "../shapes/shapes.hxx"

scene3::scene3()
{
    sun = std::make_shared<compleks::lighting::light>();
    sun->pos = glm::vec3(40, 50, 40) * 20.0f;
    // sun->cut_off = glm::radians(10.0f);
    sun->direction = glm::normalize(-sun->pos);
    compleks::engine::get_lighting().add_light(sun);

    glClearColor(135 / 255.0f, 206 / 255.0f, 235 / 255.0f, 1.0f);

    plane = std::make_shared<compleks::body>("quad");
    plane->pos = { 0, 0, 0 };
    plane->scale = { 50, 50, 1 };
    plane->quat = glm::quat({ glm::radians(-90.0f), 0.0f, 0.0f });
    plane->color = { 151 / 255.0f, 255 / 255.0f, 99 / 255.0f, 1 };
    plane->create_body(std::make_unique<plane_shape>());

    cube = std::make_shared<compleks::body>("cube", 10.0f);
    cube->pos = { 0, 0.5, 0 };
    cube->create_body(std::make_unique<cube_shape>());

    gate1 = std::make_shared<compleks::body>("gate");
    gate1->pos = { -2, 0, -4 };
    gate1->scale = { 1, 1, 16 };
    gate1->color = { 1, 0.2, 0.2, 1 };
    gate1->create_body(std::make_unique<gate_shape>());

    gate2 = std::make_shared<compleks::body>("gate");
    gate2->pos = { 2, 0, -2.5 };
    gate2->scale = { 1, 1, 4 };
    gate2->color = { 0.2, 0.2, 1, 1 };
    gate2->create_body(std::make_unique<gate_shape>());

    world.dynamic_objects.push_back(cube);
    world.static_objects.push_back(plane);
    world.static_objects.push_back(gate1);
    world.static_objects.push_back(gate2);

    p1 = std::make_shared<compleks::portal>();
    p2 = std::make_shared<compleks::portal>();
    p3 = std::make_shared<compleks::portal>();
    p4 = std::make_shared<compleks::portal>();
    p5 = std::make_shared<compleks::portal>();
    p6 = std::make_shared<compleks::portal>();
    p7 = std::make_shared<compleks::portal>();
    p8 = std::make_shared<compleks::portal>();

    p1->pos = { -2, 1, -2 };
    p1->scale = { 1, 2, 1 };
    p2->pos = { 2, 1, -2.01 };
    p2->scale = { 1, 2, 1 };
    p2->quat = glm::quat({ 0.0f, glm::radians(180.0f), 0.0f });

    p3->pos = { 2, 1, -2 };
    p3->scale = { 1, 2, 1 };
    p4->pos = { -2, 1, -2.01 };
    p4->scale = { 1, 2, 1 };
    p4->quat = glm::quat({ 0.0f, glm::radians(180.0f), 0.0f });

    p5->pos = { -2, 1, -5.99 };
    p5->scale = { 1, 2, 1 };
    p6->pos = { 2, 1, -3 };
    p6->scale = { 1, 2, 1 };
    p6->quat = glm::quat({ 0.0f, glm::radians(180.0f), 0.0f });

    p7->pos = { 2, 1, -2.99 };
    p7->scale = { 1, 2, 1 };
    p8->pos = { -2, 1, -6 };
    p8->scale = { 1, 2, 1 };
    p8->quat = glm::quat({ 0.0f, glm::radians(180.0f), 0.0f });

    p1->set_target(p2);
    p2->set_target(p1);

    p3->set_target(p4);
    p4->set_target(p3);

    p5->set_target(p6);
    p6->set_target(p5);

    p7->set_target(p8);
    p8->set_target(p7);

    p2->sub_portals.push_back(p7);
    p4->sub_portals.push_back(p5);
    p5->sub_portals.push_back(p4);
    p7->sub_portals.push_back(p2);

    // Outside
    world.portals.push_back(p1);
    world.portals.push_back(p3);
    world.portals.push_back(p6);
    world.portals.push_back(p8);

    // Inside
    world.portals.push_back(p2);
    world.portals.push_back(p4);
    world.portals.push_back(p7);
    world.portals.push_back(p5);
}
