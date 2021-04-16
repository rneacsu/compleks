#include "engine.hxx"

#include <stdexcept>
#include <string>

namespace compleks {

engine thread_local *engine::instance;

engine::engine()
{
    if (instance) {
        throw std::runtime_error("Only one window can be created per thread");
    }
    logger::info("Creating game engine");
    instance = this;

    wrld = std::make_unique<world>();

    glClearColor(0, 0, 0, 1);

    glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);
    glLineWidth(1);
    glPointSize(3);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    generate_primitives();
}

engine::~engine()
{
    logger::info("Destroying game engine");
    wrld = nullptr;
    instance = nullptr;
}

engine &engine::get()
{
    return *instance;
}

program &engine::get_program()
{
    return instance->prog;
}

library<mesh> &engine::get_meshes()
{
    return instance->meshes;
}

lighting &engine::get_lighting()
{
    return instance->light;
}

physics &engine::get_physics()
{
    return instance->phys;
}

world &engine::get_world()
{
    return *instance->wrld;
}

void engine::key_down(int key, int mods)
{
    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (!focused) {
            glfwSetWindowShouldClose(ctx, true);
        } else {
            set_cursor(true);
        }
        break;
    case GLFW_KEY_F11:
        toggle_fullscreen();
        break;
    case GLFW_KEY_ENTER:
        if (mods & GLFW_MOD_ALT) {
            toggle_fullscreen();
        }
        break;
    case GLFW_KEY_F10:
        logger::info("Recompiling shaders...");
        prog.reload();
        break;
    case GLFW_KEY_F12:
        if (polygon_mode == GL_FILL) {
            glDisable(GL_CULL_FACE);
            polygon_mode = GL_LINE;
        } else if (polygon_mode == GL_LINE) {
            polygon_mode = GL_POINT;
        } else {
            glEnable(GL_CULL_FACE);
            polygon_mode = GL_FILL;
        }
        glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);

        break;
    case GLFW_KEY_V:
        vr = !vr;
        if (!fullscreen) {
            if (vr) {
                glfwSetWindowSize(ctx, width * 2, height);
                glfwSetWindowPos(ctx, x - width / 2, y);
            } else {
                glfwSetWindowSize(ctx, width / 2, height);
                glfwSetWindowPos(ctx, x + width / 4, y);
            }
        }
        break;
    }
}

void engine::mouse_down(int button)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (!focused) {
            set_cursor(false);
        }
    }
}

void engine::render(double delta)
{
    int w, h;

    glfwGetFramebufferSize(ctx, &w, &h);
    if (w == 0 || h == 0) {
        return;
    }

    wrld->render(delta, w, h, vr);
}

void engine::generate_primitives(void)
{
    std::vector<glm::vec3> v, n;
    std::vector<glm::vec2> t;
    std::vector<unsigned int> i;

    // quad
    v = { { -0.5, 0.5, 0 },
          { 0.5, 0.5, 0 },
          { -0.5, -0.5, 0 },
          { 0.5, -0.5, 0 } };
    n = { { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0, 1 } };
    t = { { 0, 1 }, { 1, 1 }, { 0, 0 }, { 1, 0 } };
    i = { 0, 2, 1, 2, 3, 1 };
    meshes.add("quad", v, n, t, i);
}

}
