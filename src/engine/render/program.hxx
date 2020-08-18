#ifndef __COMPLEKS_RENDER_PROGRAM_HXX
#define __COMPLEKS_RENDER_PROGRAM_HXX

#include <map>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../utils/resource.hxx"
#include "shader.hxx"
#include "texture.hxx"

namespace compleks {

class program {
public:
    program();
    ~program();

    void update_shaders(std::unique_ptr<resource> v_shader_src = nullptr,
        std::unique_ptr<resource> f_shader_src = nullptr);

    void set(std::string var, int value);
    void set(std::string var, unsigned int value);
    void set(std::string var, float value);
    void set(std::string var, glm::vec3 value);
    void set(std::string var, glm::mat4 value);
    void set(std::string var, texture &tex);

private:
    GLuint id;
    bool linked = false;
    shader vertex_shader, fragment_shader;
    std::map<std::string, unsigned char> textures_map;

    GLint get_location(std::string var);
};

} // namespace compleks

#endif // __COMPLEKS_RENDER_PROGRAM_HXX