#include <iostream>
#include <array>
#include <functional>
#include <list>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "eng/gl/shader.hh"
#include "eng/gl/program.hh"
#include "eng/mesh/vertex.hh"
#include "eng/gl/buffer.hh"
#include "eng/gl/vertexarray.hh"
#include "eng/gltf/gltf.hh"
#include "eng/render/glslmaterial.hh"

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    std::cout << "GL_VENDOR:\t" << glGetString(GL_VENDOR) << '\n';
    std::cout << "GL_RENDERER:\t" << glGetString(GL_RENDERER) << '\n';
    std::cout << "GL_VERSION:\t" << glGetString(GL_VERSION) << '\n';
    std::cout << "GL_SHADING_LANGUAGE_VERSION:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

    const std::filesystem::path gltf_path = "../examples/gltf/06_shadertoy/export/shadertoy.gltf";

    const tinygltf::Model model = eng::gltf::load(gltf_path.string()).value();
    const tinygltf::Node &node = model.nodes.front();
    eng::gl::VertexArray vertex_array;
    std::list<eng::gl::Buffer> vertex_buffers;
    const tinygltf::Primitive &primitive = model.meshes[node.mesh].primitives.front();
    for (const auto &[_, index] : primitive.attributes)
    {
        const tinygltf::Accessor &accessor = model.accessors.at(index);
        const tinygltf::BufferView &view = model.bufferViews.at(accessor.bufferView);
        vertex_buffers.emplace_back(view, model);
        vertex_array.add_buffer(index, vertex_buffers.back(), accessor, model);
    }

    const tinygltf::Accessor &indices_accessor = model.accessors.at(primitive.indices);
    const tinygltf::BufferView &indices_view = model.bufferViews.at(indices_accessor.bufferView);
    eng::gl::Buffer indices_buffer(indices_view, model, GL_ELEMENT_ARRAY_BUFFER);
    indices_buffer.bind();

    eng::gl::Shader vertex_shader = [&]() -> eng::gl::Shader {
        if (primitive.material < 0)
            return eng::gl::Shader::default_vertex().value();
        const tinygltf::Material &material = model.materials.at(primitive.material);
        if (!material.extras.Has("shader"))
            return eng::gl::Shader::default_vertex().value();
        const std::string &vertex_shader_filename = material.extras.Get("shader").Get("vertex").Get<std::string>();
        std::filesystem::path vertex_shader_filepath = gltf_path.parent_path() / vertex_shader_filename;
        return eng::gl::Shader::from_file(vertex_shader_filepath, GL_VERTEX_SHADER).value();
    }();

    eng::gl::Shader fragment_shader = [&]() -> eng::gl::Shader {
        if (primitive.material < 0)
            return eng::gl::Shader::default_fragment().value();
        const tinygltf::Material &material = model.materials.at(primitive.material);
        if (!material.extras.Has("shader"))
            return eng::gl::Shader::default_fragment().value();
        const std::string &fragment_shader_filename = material.extras.Get("shader").Get("fragment").Get<std::string>();
        std::filesystem::path fragment_shader_filepath = gltf_path.parent_path() / fragment_shader_filename;
        return eng::gl::Shader::from_file(fragment_shader_filepath, GL_FRAGMENT_SHADER).value();
    }();

    eng::gl::Program program = eng::gl::Program::with_shaders(vertex_shader, fragment_shader).value();

    eng::render::GLSLMaterial glsl_material(program);
    if (primitive.material >= 0)
    {
        const tinygltf::Material &material = model.materials.at(primitive.material);
        if (material.extras.Has("shader") && material.extras.Get("shader").Has("uniforms"))
        {
            const auto &uniforms = material.extras.Get("shader").Get("uniforms").Get<tinygltf::Value::Array>();
            for (const auto &uniform : uniforms)
            {
                const std::string &name = uniform.Get("name").Get<std::string>();
                auto value = uniform.Get("value").Get<tinygltf::Value::Array>();
                const std::string &type = uniform.Get("type").Get<std::string>();

                // Replace the hardcoded values in the gltf file with the actual dynamic values
                // if (name == "time" || name == "iTime")
                //     value.at(0) = tinygltf::Value(glfwGetTime());
                // else if (name == "iResolution")
                // {
                //     int width, height;
                //     glfwGetWindowSize(window, &width, &height);
                //     value.at(0) = tinygltf::Value(double(width));
                //     value.at(1) = tinygltf::Value(double(height));
                // }
                // else if (name == "iFrame")
                // {
                //     static int frame = 0;
                //     value.at(0) = tinygltf::Value(frame++);
                // }

                glsl_material.add_uniform(name, type, value);
            }
        }
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(.32f, .85f, .58f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glsl_material.use();
        program.set_uniform("time", float(glfwGetTime()));
        program.set_uniform("iTime",float(glfwGetTime()));
        {
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            program.set_uniform("iResolution", float(width), float(height), 0.f);
        }
        {
            static int frame = 0;
            program.set_uniform("iFrame", frame++);
        }
        vertex_array.bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_accessor.count),
                       indices_accessor.componentType, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}