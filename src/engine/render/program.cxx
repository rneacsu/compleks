#include "program.hxx"

#include <string>

#include <glm/gtc/type_ptr.hpp>

#include "../utils/logger.hxx"

using namespace std::literals;

namespace compleks {

program::program()
    : vertex_shader(shader::VERTEX)
    , fragment_shader(shader::FRAGMENT)
{
    id = glCreateProgram();
    glAttachShader(id, vertex_shader.id);
    glAttachShader(id, fragment_shader.id);

    update_shaders(
        std::make_unique<compleks::resource>("res/vertex_shader.glsl"),
        std::make_unique<compleks::resource>("res/fragment_shader.glsl"));
}

program::~program()
{
    glDetachShader(id, vertex_shader.id);
    glDetachShader(id, fragment_shader.id);
    glDeleteProgram(id);
}

void program::update_shaders(std::unique_ptr<resource> v_shader_src,
    std::unique_ptr<resource> f_shader_src)
{
    if (v_shader_src && f_shader_src) {
        bool v_ok = vertex_shader.compile(std::move(v_shader_src));
        bool f_ok = fragment_shader.compile(std::move(f_shader_src));

        if (!v_ok || !f_ok) {
            return;
        }
    } else if (!vertex_shader.compile() || !fragment_shader.compile()) {
        return;
    }
    glLinkProgram(id);

    int result, length;
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
    if (!result) {
        auto msg = std::make_unique<char[]>(length);
        glGetProgramInfoLog(id, length, NULL, msg.get());
        logger::error("Linking error:\n"s + msg.get());

        return;
    }

    linked = true;
    glUseProgram(id);
}

GLint program::get_location(std::string var)
{
    GLint loc = glGetUniformLocation(id, var.c_str());

    if (loc == -1) {
        logger::warn("Invalid uniform variable: " + var);
    }

    return loc;
}

void program::set(std::string var, int value)
{
    if (!linked)
        return;
    glUniform1i(get_location(var), value);
}

void program::set(std::string var, unsigned int value)
{
    if (!linked)
        return;
    glUniform1ui(get_location(var), value);
}

void program::set(std::string var, float value)
{
    if (!linked)
        return;
    glUniform1f(get_location(var), value);
}

void program::set(std::string var, glm::vec3 value)
{
    if (!linked)
        return;
    glUniform3fv(get_location(var), 1, glm::value_ptr(value));
}

void program::set(std::string var, glm::mat4 value)
{
    if (!linked)
        return;
    glUniformMatrix4fv(get_location(var), 1, GL_FALSE, glm::value_ptr(value));
}

void program::set(std::string var, texture &tex)
{
    if (!linked)
        return;

    auto pos = textures_map.find(var);

    unsigned char index;
    if (pos == textures_map.end()) {
        index = textures_map[var] = (unsigned char)textures_map.size();
    } else {
        index = pos->second;
    }

    set(var, index);
    glActiveTexture(GL_TEXTURE0 + index);
    tex.bind();
}

} // namespace compleks
