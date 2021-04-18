#ifndef COMPLEKS_CORE_ENGINE_HXX
#define COMPLEKS_CORE_ENGINE_HXX

#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../physics/physics.hxx"
#include "../render/lighting.hxx"
#include "../render/mesh.hxx"
#include "../render/program.hxx"
#include "../utils/library.hxx"
#include "../world/world.hxx"
#include "context.hxx"

namespace compleks {

class engine : public context {
public:
    typedef std::function<std::unique_ptr<shape>()> shape_generator_t;

    engine();
    ~engine();

    void key_down(int key, int mods) override;
    void mouse_down(int button) override;

    void render(double delta) override;

    static engine &get(void);
    static program &get_program(void);
    static library<mesh> &get_meshes(void);
    static library<shape_generator_t> &get_shapes(void);
    static lighting &get_lighting(void);
    static physics &get_physics(void);
    static world &get_world(void);

private:
    library<mesh> meshes;
    library<shape_generator_t> shapes;
    program prog;
    lighting light;
    physics phys;
    std::unique_ptr<world> wrld;

    GLenum polygon_mode = GL_FILL;
    bool vr = false;

    static thread_local engine *instance;

    void generate_primitives(void);
};

}

#endif
