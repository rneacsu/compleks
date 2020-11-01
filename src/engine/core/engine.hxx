#ifndef COMPLEKS_CORE_ENGINE_HXX
#define COMPLEKS_CORE_ENGINE_HXX

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../render/lighting.hxx"
#include "../render/mesh.hxx"
#include "../render/program.hxx"
#include "../utils/library.hxx"
#include "context.hxx"

namespace compleks {

class engine : public context {
public:
    library<mesh> meshes;
    program prog;
    lighting light;

    engine();
    ~engine();

    static program &get_program(void);
    static library<mesh> &get_meshes(void);
    static lighting &get_lighting(void);

private:
    static thread_local engine *instance;
};

}

#endif
