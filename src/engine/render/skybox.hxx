#pragma once

#include <string>

#include "program.hxx"
#include "texture.hxx"

namespace compleks {

class engine;

class skybox {
public:
    skybox();
    ~skybox();

    void load(std::string dir, std::string ext);
    void render(glm::mat4 view, glm::mat4 proj);
    void unload(void);

private:
    bool loaded;
    program skybox_program;
    texture skybox_texture;

    friend class engine;
};

}
