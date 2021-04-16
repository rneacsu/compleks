#pragma once

#include <compleks.h>

class scene5 : public compleks::scene {
public:
    scene5();

private:
    std::shared_ptr<compleks::lighting::light> sun;
    std::shared_ptr<compleks::body> cube, plane, gate1, gate2, gate3, gate4;
    std::shared_ptr<compleks::portal> p1, p2;
};
