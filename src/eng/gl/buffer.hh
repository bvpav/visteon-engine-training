#pragma once

#include <cstddef>
#include <array>

#include <glad/glad.h>

namespace eng::mesh {
struct Vertex;
}

namespace eng::gl {

class Buffer
{
public:
    explicit Buffer(const std::array<eng::mesh::Vertex, 3> &vertices);
    Buffer(const Buffer &) = delete;
    ~Buffer() { glDeleteBuffers(1, &m_id); }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

private:
    Buffer() : m_id(0) { glGenBuffers(1, &m_id); }

    GLuint m_id;
};
}