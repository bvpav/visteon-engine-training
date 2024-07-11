#pragma once

#include <cstddef>
#include <utility>

#include <glad/glad.h>
#include <tiny_gltf.h>

#include "forward.hh"
#include "../mesh/vertex.hh"

namespace eng::gl {

class VertexArray
{
public:
    VertexArray();
    VertexArray(const VertexArray &) = delete;
    VertexArray(VertexArray &&other) noexcept;
    ~VertexArray();

    void add_buffer(GLuint index, const Buffer &vertex_buffer, const tinygltf::Accessor &accessor, const tinygltf::Model &model);

    void bind() const;

private:

    GLuint m_id;
};

}