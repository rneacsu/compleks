#pragma once

#include <compleks.h>

class gate_shape : public compleks::shape {
public:
    gate_shape();

private:
    compleks::shape wall_left;
    compleks::shape wall_right;
    compleks::shape ceil;
};
