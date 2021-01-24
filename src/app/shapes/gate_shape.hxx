#pragma once

#include <compleks.h>

#include <memory>

class gate_shape : public compleks::shape {
public:
    gate_shape(float width, float height, float length);

private:
    compleks::shape wall;
    compleks::shape ceil;
};
