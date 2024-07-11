#include "vertexarray.hh"

#include "buffer.hh"

namespace eng::gl {

VertexArray::VertexArray()
    : m_id(0)
{
    glGenVertexArrays(1, &m_id);
}

VertexArray::VertexArray(VertexArray &&other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0;
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::add_buffer(GLuint index, const Buffer &vertex_buffer, const tinygltf::Accessor &accessor, const tinygltf::Model &model)
{
    bind();
    vertex_buffer.bind();
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, accessor.type, accessor.componentType, accessor.normalized,
                          model.bufferViews.at(accessor.bufferView).byteStride,
                          reinterpret_cast<void *>(accessor.byteOffset));
}

}