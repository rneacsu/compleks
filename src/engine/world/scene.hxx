#pragma once

#include "../window/input_listener.hxx"

namespace compleks {

class world;
class body;

class scene : public input_listener {
public:
    scene(world &);
    virtual ~scene();

    virtual void object_mouse_down(body &obj, int button);

    void mouse_down(int button) override;
    void update(double delta);

protected:
    world &world;
    body *dragged_object = nullptr;
    bool release = false;

private:
    body *object_ray_test(void);
};

} // namespace compleks
