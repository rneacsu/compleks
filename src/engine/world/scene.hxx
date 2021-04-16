#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "../window/input_listener.hxx"

namespace compleks {

class world;
class body;

class scene : public input_listener {
public:
    const float GRAB_DISTANCE = 2.0f;
    const float GRAB_FORCE = 250.0f;

    scene(void);
    virtual ~scene();

    virtual void object_mouse_down(body &obj, int button);

    void mouse_down(int button) override;
    void key_down(int key, int mods) override;

    void update(double delta);

protected:
    world &world;
    std::shared_ptr<body> dragged_object;

private:
    std::shared_ptr<body> object_ray_test(void);
    void apply_drag_force(body &b, glm::vec3 target_pos);
};

} // namespace compleks
