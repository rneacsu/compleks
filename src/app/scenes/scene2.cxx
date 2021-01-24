#include "scene2.hxx"

#include "../shapes/gate_shape.hxx"

scene2::scene2(compleks::world &world)
    : scene(world)
{
    sun = std::make_shared<compleks::lighting::light>();
    sun->pos = glm::vec3(40, 50, 40) * 20.0f;
    // sun->cut_off = glm::radians(10.0f);
    sun->direction = glm::normalize(-sun->pos);
    compleks::engine::get_lighting().add_light(sun);

    glClearColor(135 / 255.0f, 206 / 255.0f, 235 / 255.0f, 1.0f);

    std::shared_ptr<compleks::shape> shape;
    std::unique_ptr<btCompoundShape> comp_shape;

    plane = std::make_shared<compleks::body>("quad");
    plane->pos = { 0, 0, 0 };
    plane->scale = { 50, 50, 1 };
    plane->quat = glm::quat({ glm::radians(-90.0f), 0.0f, 0.0f });
    plane->color = { 151 / 255.0f, 255 / 255.0f, 99 / 255.0f };
    shape = std::make_shared<compleks::shape>(
        std::make_unique<btBoxShape>(btVector3(25.0f, 25.0f, 0.01f)));
    plane->create_body(shape);

    cube = std::make_shared<compleks::body>("cube");
    cube->pos = { 0, 0.5, 0 };
    cube->mass = 10;
    shape = std::make_shared<compleks::shape>(
        std::make_unique<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f)));
    cube->create_body(shape);

    gate1 = std::make_shared<compleks::body>("gate");
    gate1->pos = { -3, 0, 0 };
    gate1->scale = { 1, 1, 1 };
    gate1->color = { 1, 0.2, 0.2 };
    gate1->quat = glm::quat({ 0.0f, glm::radians(90.0f), 0.0f });
    gate1->create_body(std::make_shared<gate_shape>(1.0f, 2.0f, 0.25f));

    gate2 = std::make_shared<compleks::body>("gate");
    gate2->pos = { 3, 0, 0 };
    gate2->scale = { 1, 1, 1 };
    gate2->color = { 0.2, 0.2, 1 };
    gate2->quat = glm::quat({ 0.0f, glm::radians(-90.0f), 0.0f });
    gate2->create_body(std::make_shared<gate_shape>(1.0f, 2.0f, 0.25f));

    world.objects.push_back(cube);
    world.objects.push_back(plane);
    world.objects.push_back(gate1);
    world.objects.push_back(gate2);

    p1 = std::make_shared<compleks::portal>();
    p2 = std::make_shared<compleks::portal>();

    p1->pos = { -3, 1, 0 };
    p1->scale = { 1, 2, 1 };
    p1->quat = gate1->quat;
    p2->pos = { 3, 1, 0 };
    p2->scale = { 1, 2, 1 };
    p2->quat = gate2->quat;

    p1->set_target(p2);
    p2->set_target(p1);

    p1->get_sub_portals().push_back(p2);
    p2->get_sub_portals().push_back(p1);

    world.portals.push_back(p1);
    world.portals.push_back(p2);
}
