#pragma once

namespace compleks {

class world;

class scene {
public:
    scene(world &);
    virtual ~scene();

protected:
    world &world;
};

} // namespace compleks
