#include "scene_loader.hxx"

#include "../core/engine.hxx"
#include "scene.hxx"

namespace compleks {

scene_loader::scene_loader(scene &s)
    : s(s)
{
}

scene_loader::~scene_loader()
{
}

glm::vec3 scene_loader::get_pos(nlohmann::ordered_json &j)
{
    glm::vec3 p(0);

    if (j.contains("x")) {
        p.x = j["x"];
    }
    if (j.contains("y")) {
        p.y = j["y"];
    }
    if (j.contains("z")) {
        p.z = j["z"];
    }

    return p;
}

glm::vec3 scene_loader::get_scale(nlohmann::ordered_json &j)
{
    if (j.is_number()) {
        return glm::vec3(j);
    }

    glm::vec3 scale(1);

    if (j.contains("x")) {
        scale.x = j["x"];
    }
    if (j.contains("y")) {
        scale.y = j["y"];
    }
    if (j.contains("z")) {
        scale.z = j["z"];
    }

    return scale;
}

glm::vec4 scene_loader::get_color(nlohmann::ordered_json &j)
{
    glm::vec4 c(1);

    if (j.contains("r")) {
        c.r = j["r"] / 255.0f;
    }
    if (j.contains("g")) {
        c.g = j["g"] / 255.0f;
    }
    if (j.contains("b")) {
        c.b = j["b"] / 255.0f;
    }

    return c;
}

glm::quat scene_loader::get_quat(nlohmann::ordered_json &j)
{
    glm::vec3 euler(0);

    if (j.contains("pitch")) {
        euler.x = glm::radians<float>(j["pitch"]);
    }
    if (j.contains("yaw")) {
        euler.y = glm::radians<float>(j["yaw"]);
    }
    if (j.contains("roll")) {
        euler.z = glm::radians<float>(j["roll"]);
    }

    return glm::quat(euler);
}

void scene_loader::load_lights(nlohmann::ordered_json &j)
{
    for (auto &[id, jl] : j.items()) {
        if (!jl.is_object()) {
            throw std::runtime_error("Invalid light json");
        }

        auto l = std::make_shared<lighting::light>();

        if (jl.contains("pos")) {
            l->pos = get_pos(jl["pos"]);
        }
        if (jl.contains("cutoff")) {
            l->cut_off = glm::radians<float>(jl["cutoff"]);
        }
        if (jl.contains("dir")) {
            l->direction = get_pos(jl["dir"]);
        } else {
            l->direction = glm::normalize(-l->pos);
        }
        if (jl.contains("ambient")) {
            l->ambient = get_color(jl["ambient"]);
        }
        if (jl.contains("diffuse")) {
            l->diffuse = get_color(jl["diffuse"]);
        }
        if (jl.contains("specular")) {
            l->specular = get_color(jl["specular"]);
        }
        if (jl.contains("attenuation")) {
            l->attenuation = get_pos(jl["attenuation"]);
        }
        engine::get_lighting().add_light(l);
        s.lights.emplace(id, l);
    }
}

void scene_loader::load_bodies(nlohmann::ordered_json &j)
{
    for (auto &[id, jb] : j.items()) {
        std::string mesh_id = "";
        float mass = 0;

        if (!jb.is_object()) {
            throw std::runtime_error("Invalid body json");
        }

        if (jb.contains("mesh")) {
            mesh_id = jb["mesh"];
        }
        if (jb.contains("mass")) {
            mass = jb["mass"];
        }

        auto b = std::make_shared<body>(mesh_id, mass);

        s.bodies.emplace(id, b);

        if (jb.contains("pos")) {
            b->pos = get_pos(jb["pos"]);
        }
        if (jb.contains("scale")) {
            b->scale = get_scale(jb["scale"]);
        }
        if (jb.contains("quat")) {
            b->quat = get_quat(jb["quat"]);
        }
        if (jb.contains("color")) {
            b->color = get_color(jb["color"]);
        }

        if (jb.contains("shape")) {
            if (!engine::get_shapes().exists(jb["shape"])) {
                throw std::runtime_error("Invalid shape spcified");
            }
            b->create_body(engine::get_shapes().get(jb["shape"])());
        }

        if (mass) {
            s.world.dynamic_objects.push_back(b);
        } else {
            s.world.static_objects.push_back(b);
        }
    }
}

void scene_loader::load_portals(nlohmann::ordered_json &j)
{
    for (auto &[id, jp] : j.items()) {
        if (!jp.is_object()) {
            throw std::runtime_error("Invalid portal json");
        }

        auto p = std::make_shared<portal>();

        s.portals.emplace(id, p);

        if (jp.contains("pos")) {
            p->pos = get_pos(jp["pos"]);
        }
        if (jp.contains("scale")) {
            p->scale = get_scale(jp["scale"]);
        }
        if (jp.contains("quat")) {
            p->quat = get_quat(jp["quat"]);
        }

        s.world.portals.push_back(p);
    }
    for (auto &[id, jp] : j.items()) {
        auto &p = s.portals[id];
        if (jp.contains("target")) {
            std::string target_id = jp["target"];
            auto target = s.portals.find(target_id);

            if (target == s.portals.end()) {
                throw std::runtime_error("Invalid portal target");
            }
            p->set_target(target->second);
        }
        if (jp.contains("subPortals") && jp["subPortals"].is_array()) {
            for (auto &sub_portal_id : jp["subPortals"]) {
                auto sub_portal = s.portals.find(sub_portal_id);

                if (sub_portal == s.portals.end()) {
                    throw std::runtime_error("Invalid sub portal");
                }
                p->sub_portals.push_back(sub_portal->second);
            }
        }
    }
}

void scene_loader::load(nlohmann::ordered_json &j)
{
    if (!j.is_object()) {
        throw std::runtime_error("Invalid scene json");
    }

    if (j.contains("sky")) {
        glm::vec4 sky = get_color(j["sky"]);

        glClearColor(sky.r, sky.g, sky.b, sky.a);
    }

    if (j.contains("camera")) {
        glm::vec3 pos = get_pos(j["camera"]);

        s.world.camera.set_position(pos, pos);
    }

    if (j.contains("bodies") && j["bodies"].is_object()) {
        load_bodies(j["bodies"]);
    }
    if (j.contains("lights") && j["lights"].is_object()) {
        load_lights(j["lights"]);
    }
    if (j.contains("portals") && j["portals"].is_object()) {
        load_portals(j["portals"]);
    }
}

}
