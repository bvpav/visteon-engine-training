#include "shader.hh"

#include <iostream>
#include <vector>

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
        GLint log_length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> info_log(log_length);
        glGetShaderInfoLog(id, sizeof info_log, nullptr, info_log.data());
        std::cerr << "error compiling shader: \n"
                  << info_log.data() << '\n';
        return std::nullopt;
    }
    return std::move(Shader(id));
}

}