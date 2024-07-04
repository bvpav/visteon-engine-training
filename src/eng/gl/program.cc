#include "program.hh"

#include "shader.hh"

namespace eng::gl {

static std::optional<Program> with_shaders(const Shader &vertex_shader, const Shader &fragment_shader)
{
    GLuint id = glCreateProgram();
    glAttachShader(id, vertex_shader.id());
    glAttachShader(id, fragment_shader.id());
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &has_linked_successfully);
}


}