#pragma once

#include <string>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace compleks {

class scene;

class scene_loader {
public:
    scene_loader(scene &s);
    ~scene_loader();

    void load(nlohmann::ordered_json &j);

    static glm::vec3 get_pos(nlohmann::ordered_json &j);
    static glm::vec3 get_scale(nlohmann::ordered_json &j);
    static glm::vec4 get_color(nlohmann::ordered_json &j);
    static glm::quat get_quat(nlohmann::ordered_json &j);

private:
    void load_lights(nlohmann::ordered_json &j);
    void load_bodies(nlohmann::ordered_json &j);
    void load_portals(nlohmann::ordered_json &j);

    scene &s;
};

}
