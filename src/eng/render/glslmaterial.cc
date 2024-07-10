#include "glslmaterial.hh"

#include "../gl/program.hh"

namespace eng::render {

GLSLMaterial::GLSLMaterial(gl::Program &program)
    : m_program(program)
{
}

void GLSLMaterial::add_uniform(const std::string &name, const std::string &type, const tinygltf::Value::Array &value)
{
    if (type == "Float")
        m_uniform_floats[name] = float(value.at(0).Get<double>());
    else if (type == "Int")
        m_uniform_ints[name] = value.at(0).Get<int>();
    else if (type == "Vector3")
        m_uniform_vec3s[name] = {float(value.at(0).Get<double>()),
                                 float(value.at(1).Get<double>()),
                                 float(value.at(2).Get<double>())};
    else if (type == "Vector4")
        m_uniform_vec4s[name] = {float(value.at(0).Get<double>()),
                                 float(value.at(1).Get<double>()),
                                 float(value.at(2).Get<double>()),
                                 float(value.at(3).Get<double>())};
}

void GLSLMaterial::use()
{
    m_program.use();
    for (const auto &[name, value] : m_uniform_floats)
        m_program.set_uniform(name, value);
    for (const auto &[name, value] : m_uniform_ints)
        m_program.set_uniform(name, value);
    for (const auto &[name, value] : m_uniform_vec3s)
        m_program.set_uniform(name, value[0], value[1], value[2]);
    for (const auto &[name, value] : m_uniform_vec4s)
        m_program.set_uniform(name, value[0], value[1], value[2], value[3]);
}

gl::Program &GLSLMaterial::program()
{
    return m_program;
}

}