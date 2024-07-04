#pragma once

#include <glad/glad.h>
#include <optional>

namespace eng::gl {

class Shader
{
public:
    static std::optional<Shader> from_src(const char *src, GLenum type);

    Shader(const Shader &) = delete;

    Shader(Shader &&other) noexcept
        : m_id(other.m_id)
    {
        other.m_id = 0;
    }

    ~Shader()
    {
        if (m_id)
            glDeleteShader(m_id);
    }

    [[nodiscard]] GLuint id() const { return m_id; }

private:
    explicit Shader(GLuint id) : m_id(id) {}

    GLuint m_id;
};

}