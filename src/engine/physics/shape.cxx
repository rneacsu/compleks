#include "shape.hxx"

namespace compleks {

shape::shape()
{
}

shape::~shape()
{
}

shape::shape(std::unique_ptr<btCollisionShape> s)
{
    set_shape(std::move(s));
}

void shape::set_shape(std::unique_ptr<btCollisionShape> s)
{
    bt_shape = std::move(s);
}

btCollisionShape &shape::get_shape(void)
{
    return *bt_shape;
}

} // namespace compleks
