#pragma once

#include <cstddef>
#include <array>

#include <glad/glad.h>
#include <tiny_gltf.h>

#include "../mesh/vertex.hh"

namespace eng::gl {

class Buffer
{
public:
    Buffer(const tinygltf::BufferView &, const tinygltf::Model &, GLenum target = GL_ARRAY_BUFFER);

    Buffer(const Buffer &) = delete;
    ~Buffer();

    void bind() const;

private:
    explicit Buffer(GLenum target);
    Buffer(const void *vertices, size_t size, GLenum target);

    GLuint m_id;
    GLenum m_target;
};

}