#pragma once

#include <glad/glad.h>
#include <optional>
#include <unordered_map>
#include <string>

namespace eng::gl {

class Shader;

class Program
{
public:
    static std::optional<Program> with_shaders(const Shader &vertex_shader, const Shader &fragment_shader);

    Program(const Program &) = delete;

    Program(Program &&other) noexcept
        : m_id(other.m_id)
    {
        other.m_id = 0;
    }

    ~Program()
    {
        if (m_id)
            glDeleteProgram(m_id);
    }

    void use() const
    {
        glUseProgram(m_id);
    }

    void set_uniform(const std::string &name, GLint value);
    void set_uniform(const std::string &name, GLfloat value);
    void set_uniform(const std::string &name, GLfloat x, GLfloat y, GLfloat z);
    void set_uniform(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

private:
    explicit Program(GLuint id) : m_id(id) {}

    // FIXME: this modifies the internal cache. should it reealy be const?
    std::optional<GLint> get_uniform_location(const std::string &name) const;

    GLuint m_id;
    mutable std::unordered_map<std::string, GLint> m_uniforms;
};

};