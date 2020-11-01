#ifndef __COMPLEKS_WORLD_WORLD_HXX
#define __COMPLEKS_WORLD_WORLD_HXX

#include <memory>
#include <string>
#include <vector>

#include "../core/engine.hxx"
#include "camera.hxx"
#include "object.hxx"
#include "portal.hxx"

namespace compleks {

class world : public engine {
public:
    world();
    ~world();

    void key_down(int key, int mods) override;
    void mouse_down(int button) override;

    void load_mesh(std::string id, std::string path);

    void render(double) override;
    void render_objects(glm::mat4 view, glm::mat4 proj);
    void render_portals(glm::mat4 view, glm::mat4 proj, int depth = 0,
        std::shared_ptr<portal> current_portal = nullptr);
    void render_vr(int w, int h);

    void set_transform(glm::mat4 view, glm::mat4 proj);

protected:
    std::vector<std::shared_ptr<object>> objects;
    std::vector<std::shared_ptr<portal>> portals;
    camera camera;

private:
    GLenum polygon_mode = GL_FILL;
    int max_portal_depth = 4;
    bool vr = false;
};

}

#endif // __COMPLEKS_WORLD_WORLD_HXX
