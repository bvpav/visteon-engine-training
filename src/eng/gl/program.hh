#pragma once

#include <glad/glad.h>
#include <optional>

namespace eng::gl {

struct Shader;

class Program
{
public:
    static std::optional<Program> with_shaders(const Shader &vertex_shader, const Shader &fragment_shader);

    Program(Program &&other)
        : m_id(other.m_id)
    {
        other.m_id = 0;
    }

    ~Program()
    {
        glDeleteProgram(m_id);
    }

    void use() const
    {
        glUseProgram(m_id);
    }

private:
    Program(GLuint id) : m_id(id) {}

    GLuint m_id;
};

};