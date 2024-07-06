#include "buffer.hh"

#include "../mesh/vertex.hh"

namespace eng::gl {

Buffer::Buffer(const eng::mesh::Vertex *vertices, size_t size)
    : Buffer()
{
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(eng::mesh::Vertex),
                          reinterpret_cast<void *>(offsetof(eng::mesh::Vertex, x)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof (eng::mesh::Vertex),
                          reinterpret_cast<void *>(offsetof(eng::mesh::Vertex, norm_x)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          sizeof (eng::mesh::Vertex),
                          reinterpret_cast<void *>(offsetof(eng::mesh::Vertex, u)));
}

}