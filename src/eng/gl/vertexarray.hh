#pragma once

#include <cstddef>
#include <utility>

#include <glad/glad.h>

#include "buffer.hh"
#include "../mesh/vertex.hh"

namespace eng::gl {

class VertexArray
{
public:
    VertexArray();
    VertexArray(const VertexArray &) = delete;
    VertexArray(VertexArray &&other) noexcept;
    ~VertexArray();

    template<class T>
    static VertexArray from(const Buffer &vertex_buffer);

    void bind() const;

private:
    GLuint m_id;
};

template<class T>
VertexArray VertexArray::from(const Buffer &)
{
    static_assert(false, "Unsupported type");
}

template<>
inline VertexArray VertexArray::from<eng::mesh::Vertex>(const Buffer &vertex_buffer)
{
    vertex_buffer.bind();

    VertexArray array;
    array.bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof (eng::mesh::Vertex),
                          reinterpret_cast<void *>(offsetof(eng::mesh::Vertex, x)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof (eng::mesh::Vertex),
                          reinterpret_cast<void *>(offsetof(eng::mesh::Vertex, norm_x)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          sizeof (eng::mesh::Vertex),
                          reinterpret_cast<void *>(offsetof(eng::mesh::Vertex, u)));
    return array;
}

}