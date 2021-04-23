#ifndef __COMPLEKS_WORLD_WORLD_HXX
#define __COMPLEKS_WORLD_WORLD_HXX

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "../physics/body.hxx"
#include "../render/object.hxx"
#include "camera.hxx"
#include "portal.hxx"
#include "scene.hxx"

namespace compleks {

class world {
public:
    world();
    ~world();

    std::vector<std::shared_ptr<object>> static_objects;
    std::vector<std::shared_ptr<object>> hud;
    std::vector<std::shared_ptr<body>> dynamic_objects;
    std::vector<std::shared_ptr<portal>> portals;
    camera camera;
    std::unique_ptr<scene> current_scene;

    void render(double, int w, int h, bool vr = false);
    std::shared_ptr<portal> get_closest_portal(glm::vec3 pos);
    void set_fog(float start, float end);

private:
    void set_transform(glm::mat4 view, glm::mat4 proj);
    void render_objects(glm::mat4 view, glm::mat4 proj);
    void render_portals(
        glm::mat4 view,
        glm::mat4 proj,
        int depth = 0,
        std::shared_ptr<portal> current_portal = nullptr);
    void update(double delta);

    int max_portal_depth = 5;
    glm::vec2 fog;
};

}

#endif // __COMPLEKS_WORLD_WORLD_HXX
