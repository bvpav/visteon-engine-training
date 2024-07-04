#include "buffer.hh"

#include "../mesh/vertex.hh"

namespace eng::gl {

Buffer::Buffer(const std::array<eng::mesh::Vertex, 3> &vertices)
    : Buffer()
{
    bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(eng::mesh::Vertex),
                          reinterpret_cast<void *>(offsetof(eng::mesh::Vertex, x)));
}

}