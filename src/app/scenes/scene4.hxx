#pragma once

#include <compleks.h>

class scene4 : public compleks::scene {
public:
    scene4(compleks::world &world);

private:
    std::shared_ptr<compleks::lighting::light> sun;
    std::shared_ptr<compleks::body> plane;
    std::shared_ptr<compleks::body> gate11, gate12, pillar1, cube1;
    std::shared_ptr<compleks::body> gate21, gate22, pillar2, cube2;
    std::shared_ptr<compleks::portal> p1, p2, p3, p4;
};
