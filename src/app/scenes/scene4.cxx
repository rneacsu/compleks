#include "scene4.hxx"

#include "../shapes/shapes.hxx"

scene4::scene4()
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

    world.static_objects.push_back(plane);

    // Room 1
    cube1 = std::make_shared<compleks::body>("cube", 10.0f);
    cube1->pos = { 0, 0.5, -2 };
    cube1->color = { 1.0, 0.5, 0.5, 1 };
    cube1->create_body(std::make_unique<cube_shape>());

    gate11 = std::make_shared<compleks::body>("gate");
    gate11->pos = { 0, 0, 0 };
    gate11->scale = { 4, 1.5, 32 };
    gate11->color = { 107 / 255.0f, 184 / 255.0f, 255 / 255.0f, 1 };
    gate11->create_body(std::make_unique<gate_shape>());

    gate12 = std::make_shared<compleks::body>("gate");
    gate12->pos = { 0, 0, 0 };
    gate12->scale = { 8, 1.5, 16 };
    gate12->color = { 107 / 255.0f, 184 / 255.0f, 255 / 255.0f, 1 };
    gate12->quat = glm::quat({ 0.0f, glm::radians(90.0f), 0.0f });
    gate12->create_body(std::make_unique<gate_shape>());

    pillar1 = std::make_shared<compleks::body>("cube");
    pillar1->pos = { 0, 1.5, 1.875 };
    pillar1->scale = { 0.25, 3, 0.25 };
    pillar1->create_body(std::make_unique<cube_shape>());

    world.dynamic_objects.push_back(cube1);
    world.static_objects.push_back(gate11);
    world.static_objects.push_back(gate12);
    world.static_objects.push_back(pillar1);

    // Room 2
    cube2 = std::make_shared<compleks::body>("cube", 10.0f);
    cube2->pos = { 10, 0.5, -2 };
    cube2->color = { 0.5, 0.5, 1, 1 };
    cube2->create_body(std::make_unique<cube_shape>());

    gate21 = std::make_shared<compleks::body>("gate");
    gate21->pos = { 10, 0, 0 };
    gate21->scale = { 4, 1.5, 32 };
    gate21->color = { 107 / 255.0f, 184 / 255.0f, 255 / 255.0f, 1 };
    gate21->create_body(std::make_unique<gate_shape>());

    gate22 = std::make_shared<compleks::body>("gate");
    gate22->pos = { 10, 0, 0 };
    gate22->scale = { 8, 1.5, 16 };
    gate22->color = { 107 / 255.0f, 184 / 255.0f, 255 / 255.0f, 1 };
    gate22->quat = glm::quat({ 0.0f, glm::radians(90.0f), 0.0f });
    gate22->create_body(std::make_unique<gate_shape>());

    pillar2 = std::make_shared<compleks::body>("cube");
    pillar2->pos = { 10, 1.5, 1.875 };
    pillar2->scale = { 0.25, 3, 0.25 };
    pillar2->create_body(std::make_unique<cube_shape>());

    world.dynamic_objects.push_back(cube2);
    world.static_objects.push_back(gate21);
    world.static_objects.push_back(gate22);
    world.static_objects.push_back(pillar2);

    p1 = std::make_shared<compleks::portal>();
    p2 = std::make_shared<compleks::portal>();
    p3 = std::make_shared<compleks::portal>();
    p4 = std::make_shared<compleks::portal>();

    p1->pos = { 0, 1.5, 3 };
    p1->scale = { 2, 3, 1 };
    p1->quat = glm::quat({ 0.0f, glm::radians(-90.0f), 0.0f });
    p2->pos = { 10, 1.5, 3 };
    p2->scale = { 2, 3, 1 };
    p2->quat = glm::quat({ 0.0f, glm::radians(90.0f), 0.0f });

    p3->pos = { 0, 1.5, 3 };
    p3->scale = { 2, 3, 1 };
    p3->quat = glm::quat({ 0.0f, glm::radians(90.0f), 0.0f });
    p4->pos = { 10, 1.5, 3 };
    p4->scale = { 2, 3, 1 };
    p4->quat = glm::quat({ 0.0f, glm::radians(-90.0f), 0.0f });

    p1->set_target(p2);
    p2->set_target(p1);

    p3->set_target(p4);
    p4->set_target(p3);

    world.portals.push_back(p1);
    world.portals.push_back(p2);
    world.portals.push_back(p3);
    world.portals.push_back(p4);
}
