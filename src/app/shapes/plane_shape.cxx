#include "plane_shape.hxx"

plane_shape::plane_shape()
{
    set_shape(std::make_unique<btBoxShape>(btVector3(0.5f, 0.5f, 0.01f)));
}
