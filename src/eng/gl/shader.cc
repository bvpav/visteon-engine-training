#include "shader.hh"

#include <iostream>

#include <glad/glad.h>

namespace eng::gl {

std::optional<Shader> Shader::from_src(const GLchar *src, GLenum type)
{
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    GLint has_compiled_successfully;
    glGetShaderiv(id, GL_COMPILE_STATUS, &has_compiled_successfully);
    if (!has_compiled_successfully)
    {
        char info_log[512];
        glGetShaderInfoLog(id, sizeof info_log, nullptr, info_log);
        std::cerr << "error compiling shader: \n"
                  << info_log << '\n';
        return std::nullopt;
    }
    return std::move(Shader(id));
}

}