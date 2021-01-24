#pragma once

#include <compleks.h>

class scene6 : public compleks::scene {
public:
    scene6(compleks::world &world);

private:
    std::shared_ptr<compleks::lighting::light> sun;
    std::shared_ptr<compleks::body> plane, gate1, gate2, gate3, gate4;
    std::shared_ptr<compleks::portal> p1, p2, p3, p4;
};
