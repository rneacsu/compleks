#pragma once

#include <compleks.h>

class scene2 : public compleks::scene {
public:
    scene2();

private:
    std::shared_ptr<compleks::lighting::light> sun;
    std::shared_ptr<compleks::body> cube, plane, gate1, gate2;
    std::shared_ptr<compleks::portal> p1, p2;
};
