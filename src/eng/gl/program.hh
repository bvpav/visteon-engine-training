#pragma once

#include <glad/glad.h>
#include <optional>

namespace eng::gl {

struct Shader;

class Program
{
public:
    static std::optional<Program> with_shaders(const Shader &vertex_shader, const Shader &fragment_shader);

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

private:
    explicit Program(GLuint id) : m_id(id) {}

    GLuint m_id;
};

};