#include "skybox.hxx"

#include <future>
#include <vector>

#include "../core/engine.hxx"

namespace compleks {

skybox::skybox()
    : skybox_texture(GL_TEXTURE_CUBE_MAP)
    , skybox_program(
          std::make_unique<resource>("res/shaders/sky.vs.glsl"),
          std::make_unique<resource>("res/shaders/sky.fs.glsl"))
{
    skybox_texture.bind();

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    loaded = false;
}

skybox::~skybox()
{
}

void skybox::load(std::string dir, std::string ext)
{
    skybox_texture.bind();

    std::vector<std::future<std::unique_ptr<image>>> tasks;

    for (auto s : { "px", "nx", "py", "ny", "pz", "nz" }) {
        tasks.push_back(std::async(std::launch::async, [dir, ext, s]() {
            return std::make_unique<image>(resource(dir + "/" + s + "." + ext));
        }));
    }

    int i = 0;
    for (auto &t : tasks) {
        t.wait();
        auto img = t.get();
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0,
            GL_RGBA,
            img->width,
            img->height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            img->pixels);
        i++;
    }

    loaded = true;
}

void skybox::render(glm::mat4 view, glm::mat4 proj)
{
    if (!loaded) {
        return;
    }

    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_FRONT);
    skybox_program.use();

    view = glm::mat4(glm::mat3(view));
    skybox_program.set("projection_view_matrix", proj * view);
    skybox_texture.bind();

    engine::get_meshes().get("cube").render_simple();

    glDepthFunc(GL_LESS);
    glCullFace(GL_BACK);
    engine::get_program().use();
}

void skybox::unload(void)
{
    loaded = false;
}

}
