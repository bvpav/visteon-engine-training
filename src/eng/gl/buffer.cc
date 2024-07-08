#include "buffer.hh"

#include "../mesh/vertex.hh"

namespace eng::gl {

Buffer::Buffer(GLenum target)
        : m_id(0), m_target(target)
{
    glGenBuffers(1, &m_id);
}

Buffer::Buffer(const void *vertices, size_t size, GLenum target)
        : Buffer(target)
{
    bind();
    glBufferData(m_target, static_cast<GLsizei>(size), vertices, GL_STATIC_DRAW);
}

Buffer::Buffer(const tinygltf::BufferView &buffer_view, const tinygltf::Model &model, GLenum target)
        : Buffer(model.buffers.at(buffer_view.buffer).data.data() + buffer_view.byteOffset,
                 buffer_view.byteLength, target)
{
}

Buffer::~Buffer()
{
    glDeleteBuffers(1, &m_id);
}

void Buffer::bind() const
{
    glBindBuffer(m_target, m_id);
}

}