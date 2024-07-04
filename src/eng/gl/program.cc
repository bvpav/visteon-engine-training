#include "program.hh"

#include "shader.hh"

#include <iostream>

namespace eng::gl {

std::optional<Program> Program::with_shaders(const Shader &vertex_shader, const Shader &fragment_shader)
{
    GLuint id = glCreateProgram();
    glAttachShader(id, vertex_shader.id());
    glAttachShader(id, fragment_shader.id());
    glLinkProgram(id);

    GLint has_linked_successfully;
    glGetProgramiv(id, GL_LINK_STATUS, &has_linked_successfully);
    if (!has_linked_successfully)
    {
        char info_log[512];
        glGetShaderInfoLog(id, sizeof info_log, nullptr, info_log);
        std::cerr << "error linking program: \n"
                  << info_log << '\n';
        return std::nullopt;
    }

    return std::move(Program(id));
}

}