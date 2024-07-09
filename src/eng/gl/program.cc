#include "program.hh"

#include "shader.hh"

#include <iostream>
#include <vector>

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
        GLint log_length;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> info_log(log_length);
        glGetProgramInfoLog(id, log_length, nullptr, info_log.data());
        std::cerr << "error linking program: \n"
                  << info_log.data() << '\n';
        return std::nullopt;

    }

    return std::move(Program(id));
}

std::optional<GLint> Program::get_uniform_location(const std::string &name) const
{
    GLint location;
    if (m_uniforms.find(name) != m_uniforms.end())
        location = m_uniforms.at(name);
    else
        location = m_uniforms[name] = glGetUniformLocation(m_id, name.c_str());
    
    if (location == GL_INVALID_INDEX)
        return std::nullopt;
    return location;
}

void Program::set_uniform(const std::string &name, GLfloat value) const
{
    if (auto location = get_uniform_location(name))
        glUniform1f(*location, value);
}

void Program::set_uniform(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
    if (auto location = get_uniform_location(name))
        glUniform4f(*location, x, y, z, w);
}

}