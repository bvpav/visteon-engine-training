#pragma once

#include <functional>

#include <tiny_gltf.h>

#include "../gl/forward.hh"

namespace eng::render {

class GLSLMaterial
{
public:
    GLSLMaterial(gl::Program &program);

    void add_uniform(const std::string &name, const std::string &type, const tinygltf::Value::Array &value);

    void use();

    gl::Program &program();

private:
    gl::Program &m_program;

    // TODO: use better types maybe?
    std::unordered_map<std::string, float> m_uniform_floats;
    std::unordered_map<std::string, int> m_uniform_ints;
    std::unordered_map<std::string, std::array<float, 3>> m_uniform_vec3s;
    std::unordered_map<std::string, std::array<float, 4>> m_uniform_vec4s;
};

}