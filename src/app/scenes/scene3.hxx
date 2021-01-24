#pragma once

#include <compleks.h>

class scene3 : public compleks::scene {
public:
    scene3(compleks::world &world);

private:
    std::shared_ptr<compleks::lighting::light> sun;
    std::shared_ptr<compleks::body> cube, plane, gate1, gate2;
    std::shared_ptr<compleks::portal> p1, p2, p3, p4, p5, p6, p7, p8;
};
