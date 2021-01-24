#include "gate_shape.hxx"

gate_shape::gate_shape(float width, float height, float length)
{
    float margin = 0.25f;

    wall.set_shape(std::make_unique<btBoxShape>(
        btVector3(margin / 2, height / 2, length / 2)));
    ceil.set_shape(std::make_unique<btBoxShape>(
        btVector3(margin + width / 2.0f, margin / 2, length / 2)));

    auto s = std::make_unique<btCompoundShape>();

    s->addChildShape(btTransform(btQuaternion(0, 0, 0),
                         btVector3(-(width + margin) / 2, height / 2, 0)),
        wall.get_shape());

    s->addChildShape(btTransform(btQuaternion(0, 0, 0),
                         btVector3((width + margin) / 2, height / 2, 0)),
        wall.get_shape());

    s->addChildShape(btTransform(btQuaternion(0, 0, 0),
                         btVector3(0, height + margin / 2, 0)),
        ceil.get_shape());

    set_shape(std::move(s));
}
