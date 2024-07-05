#pragma once

#include <cstddef>
#include <array>

#include <glad/glad.h>

#include "../mesh/vertex.hh"

// namespace eng::mesh {
// struct Vertex;
// }

namespace eng::gl {

class Buffer
{
public:
    template<std::size_t size>
    explicit Buffer(const std::array<eng::mesh::Vertex, size> &vertices)
        : Buffer(vertices.data(), sizeof vertices)
    {}
    Buffer(const eng::mesh::Vertex *vertices, size_t size);

    Buffer(const Buffer &) = delete;
    ~Buffer() { glDeleteBuffers(1, &m_id); }

    void bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }

private:
    Buffer() : m_id(0) { glGenBuffers(1, &m_id); }

    GLuint m_id;
};
}