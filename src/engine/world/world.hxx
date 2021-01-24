#ifndef __COMPLEKS_WORLD_WORLD_HXX
#define __COMPLEKS_WORLD_WORLD_HXX

#include <memory>
#include <string>
#include <vector>

#include "../core/engine.hxx"
#include "../physics/body.hxx"
#include "camera.hxx"
#include "object.hxx"
#include "portal.hxx"
#include "scene.hxx"

namespace compleks {

class world : public engine {
public:
    world();
    ~world();

    void key_down(int key, int mods) override;
    void mouse_down(int button) override;

    void load_mesh(std::string id, std::string path);

    template <class T> void load_scene(void)
    {
        static_assert(std::is_base_of<scene, T>::value);

        current_scene = nullptr;
        current_scene = std::make_unique<T>(*this);
    }

    void render(double) override;
    void render_objects(glm::mat4 view, glm::mat4 proj);
    void render_portals(glm::mat4 view, glm::mat4 proj, int depth = 0,
        std::shared_ptr<portal> current_portal = nullptr);
    void render_vr(int w, int h);

    void set_transform(glm::mat4 view, glm::mat4 proj);

    std::vector<std::shared_ptr<object>> objects;
    std::vector<std::shared_ptr<portal>> portals;
    camera camera;

protected:
private:
    GLenum polygon_mode = GL_FILL;
    int max_portal_depth = 5;
    bool vr = false;

    std::unique_ptr<scene> current_scene;
};

}

#endif // __COMPLEKS_WORLD_WORLD_HXX
