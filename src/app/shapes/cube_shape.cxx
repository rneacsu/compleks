#include "cube_shape.hxx"

cube_shape::cube_shape()
{
    set_shape(std::make_unique<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f)));
}
