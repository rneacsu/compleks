#include "shader.hxx"

#include <memory>
#include <stdexcept>
#include <string>

#include "../utils/logger.hxx"

namespace compleks {

shader::shader(type t)
{
    id = glCreateShader(t == VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
}

bool shader::compile(std::unique_ptr<resource> source)
{
    src = std::move(source);

    glShaderSource(id, 1, (char **)&src->ptr, (int *)&src->size);
    glCompileShader(id);

    int status, length;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        auto log = std::make_unique<char[]>(length);
        glGetShaderInfoLog(id, length, NULL, log.get());

        logger::error(std::string("Shader compile error:\n") + log.get());
    }

    return status;
}

bool shader::reload()
{
    src->reload();

    return compile(std::move(src));
}

shader::~shader()
{
    glDeleteShader(id);
}

} // namespace compleks
