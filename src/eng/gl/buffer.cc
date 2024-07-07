#include "buffer.hh"

#include "../mesh/vertex.hh"

namespace eng::gl {

Buffer::Buffer(const void *vertices, size_t size)
    : Buffer()
{
    bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(size), vertices, GL_STATIC_DRAW);
}

}