#include "vertexarray.hh"

#include "../mesh/vertex.hh"

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

}