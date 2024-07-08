#pragma once

#include <glad/glad.h>
#include <optional>
#include <filesystem>

namespace eng::gl {

class Shader
{
public:
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

    static std::optional<Shader> from_src(const char *src, GLenum type);
    static std::optional<Shader> from_file(const std::filesystem::path &path, GLenum type);

    [[nodiscard]] GLuint id() const { return m_id; }

private:
    explicit Shader(GLuint id) : m_id(id) {}

    GLuint m_id;
};

}