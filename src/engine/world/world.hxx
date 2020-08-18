#ifndef __COMPLEKS_WORLD_WORLD_HXX
#define __COMPLEKS_WORLD_WORLD_HXX

#include <memory>
#include <vector>
#include <string>
#include <utility>

#include "../utils/resource.hxx"
#include "../input/input_window.hxx"
#include "../render/mesh.hxx"
#include "../render/shader.hxx"
#include "../render/lighting.hxx"
#include "camera.hxx"
#include "object.hxx"

namespace compleks
{

class world : public input_window {
public:
    world();
    ~world();

    void key_down(int key, int mods) override;
    void mouse_down(int button) override;
    void render(double) override;

protected:
    std::vector<object *> objects;
    program program;
    camera camera;
    lighting lighting;
private:
    GLenum polygon_mode = GL_FILL;
};

}

#endif // __COMPLEKS_WORLD_WORLD_HXX