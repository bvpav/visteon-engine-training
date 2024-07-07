#include "buffer.hh"

#include "../mesh/vertex.hh"

namespace eng::gl {

Buffer::Buffer(const eng::mesh::Vertex *vertices, size_t size)
    : Buffer()
{
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

}