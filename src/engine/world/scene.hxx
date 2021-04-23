#pragma once

#include <map>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "../physics/body.hxx"
#include "../render/lighting.hxx"
#include "../window/input_listener.hxx"
#include "portal.hxx"
#include "scene_loader.hxx"

namespace compleks {

class world;

class scene : public input_listener {
public:
    const float GRAB_DISTANCE = 2.0f;
    const float GRAB_FORCE = 5.0f;

    nlohmann::ordered_json config;
    std::map<std::string, std::shared_ptr<lighting::light>> lights;
    std::map<std::string, std::shared_ptr<body>> bodies;
    std::map<std::string, std::shared_ptr<portal>> portals;

    scene(void);
    virtual ~scene();

    virtual void object_mouse_down(body &obj, int button);

    void mouse_down(int button) override;
    void key_down(int key, int mods) override;

    void load_scene(std::string path);

    void update(double delta);

protected:
    world &world;
    std::shared_ptr<body> dragged_object;

private:
    friend class scene_loader;

    std::shared_ptr<body> object_ray_test(void);
    void apply_drag_force(body &b, glm::vec3 target_pos, double delta);
};

} // namespace compleks
