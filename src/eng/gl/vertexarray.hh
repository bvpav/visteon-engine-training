#pragma once

#include <cstddef>
#include <utility>

#include <glad/glad.h>
#include <tiny_gltf.h>

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

    template<class T>
    static VertexArray from(const Buffer &vertex_buffer, const T &);

    void bind() const;

private:
    template<class T>
    void push_attribute(GLuint index, GLint size, GLsizei stride, const void *offset);

    template<class T>
    void push_attribute(GLuint index, GLint size, GLsizei stride, size_t offset)
    {
        push_attribute<T>(index, size, stride, reinterpret_cast<void *>(offset));
    }

    GLuint m_id;
};

template<class T>
void VertexArray::push_attribute(GLuint index, GLint size, GLsizei stride, const void *offset)
{
    static_assert(false, "Unsupported type");
}

template<class T>
VertexArray VertexArray::from(const Buffer &vertex_buffer, const T &)
{
    static_assert(false, "Unsupported type");
    return {};
}

template<>
inline void VertexArray::push_attribute<float>(GLuint index, GLint size, GLsizei stride, const void *offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE,
                          stride, offset);
}

template<class T>
VertexArray VertexArray::from(const Buffer &)
{
    static_assert(false, "Unsupported type");
    return {};
}

template<>
inline VertexArray VertexArray::from<eng::mesh::Vertex>(const Buffer &vertex_buffer)
{
    vertex_buffer.bind();

    VertexArray array;
    array.bind();

    array.push_attribute<float>(0, 3, sizeof (eng::mesh::Vertex), offsetof(eng::mesh::Vertex, x));
    array.push_attribute<float>(1, 3, sizeof (eng::mesh::Vertex), offsetof(eng::mesh::Vertex, norm_x));
    array.push_attribute<float>(2, 2, sizeof (eng::mesh::Vertex), offsetof(eng::mesh::Vertex, u));
    return array;
}

template<>
inline VertexArray VertexArray::from<eng::mesh::WeirdVertex>(const Buffer &vertex_buffer)
{
    vertex_buffer.bind();

    VertexArray array;
    array.bind();

    array.push_attribute<float>(0, 2, sizeof (eng::mesh::WeirdVertex), offsetof(eng::mesh::Vertex, u));
    array.push_attribute<float>(1, 3, sizeof (eng::mesh::WeirdVertex), offsetof(eng::mesh::Vertex, norm_x));
    array.push_attribute<float>(2, 3, sizeof (eng::mesh::WeirdVertex), offsetof(eng::mesh::Vertex, x));
    return array;
}

template<>
inline VertexArray VertexArray::from(const Buffer &vertex_buffer, const tinygltf::BufferView &buffer_view)
{
    vertex_buffer.bind();

    VertexArray array;
    array.bind();

    array.push_attribute<float>(0, 3, static_cast<GLsizei>(buffer_view.byteStride), buffer_view.byteOffset);
    return array;
}

}