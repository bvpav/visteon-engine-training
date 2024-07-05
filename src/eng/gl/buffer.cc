#include "buffer.hh"

#include "../mesh/vertex.hh"

namespace eng::gl {

Buffer::Buffer(const eng::mesh::Vertex *vertices, size_t size)
{
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(eng::mesh::Vertex),
                          reinterpret_cast<void *>(offsetof(eng::mesh::Vertex, x)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof (eng::mesh::Vertex),
                          reinterpret_cast<void *>(offsetof(eng::mesh::Vertex, norm_x)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                          sizeof (eng::mesh::Vertex),
                          reinterpret_cast<void *>(offsetof(eng::mesh::Vertex, u)));
}

}