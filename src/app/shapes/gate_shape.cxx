#include "gate_shape.hxx"

gate_shape::gate_shape()
{
    float margin = 0.25f;

    btVector3 wall_hl(margin / 2, 1.0f, margin / 2);
    btVector3 ceil_hl(margin + 0.5f, margin / 2, margin / 2);

    wall_left.set_shape(std::make_unique<btBoxShape>(wall_hl));
    wall_right.set_shape(std::make_unique<btBoxShape>(wall_hl));
    ceil.set_shape(std::make_unique<btBoxShape>(ceil_hl));

    auto s = std::make_unique<btCompoundShape>();

    s->addChildShape(
        compleks::physics::to_bt(
            glm::vec3(-(1 + margin) / 2, 1.0f, 0),
            glm::quat({ 0, 0, 0 })),
        &wall_left.get_shape());

    s->addChildShape(
        compleks::physics::to_bt(
            glm::vec3((1 + margin) / 2, 1.0f, 0),
            glm::quat({ 0, 0, 0 })),
        &wall_right.get_shape());

    s->addChildShape(
        compleks::physics::to_bt(
            glm::vec3(0, 2.0f + margin / 2, 0),
            glm::quat({ 0, 0, 0 })),
        &ceil.get_shape());

    set_shape(std::move(s));
}
