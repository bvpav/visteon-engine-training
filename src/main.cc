#include <iostream>
#include <array>
#include <functional>
#include <list>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "eng/gl/shader.hh"
#include "eng/gl/program.hh"
#include "eng/mesh/vertex.hh"
#include "eng/gl/buffer.hh"
#include "eng/gl/vertexarray.hh"
#include "eng/gltf/gltf.hh"

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

    const GLchar *vertex_shader_src = R"(
        #version 330 core
        layout (location = 0) in vec3 a_pos;

        void main()
        {
            gl_Position = vec4(a_pos, 1.0);
        }
    )";
    eng::gl::Shader vertex_shader = eng::gl::Shader::from_src(vertex_shader_src, GL_VERTEX_SHADER).value();

    const GLchar *fragment_shader_src = R"(
        #version 330 core
        out vec4 out_frag_color;

        void main()
        {
            out_frag_color = vec4(0.77f, 0.0f, 0.75f, 1.0f);
        }
    )";
    eng::gl::Shader fragment_shader = eng::gl::Shader::from_src(fragment_shader_src, GL_FRAGMENT_SHADER).value();

    eng::gl::Program program = eng::gl::Program::with_shaders(vertex_shader, fragment_shader).value();

    tinygltf::Model model = eng::gltf::load("../examples/gltf/01_triangle/export/triangle.gltf").value();
    const tinygltf::Node &node = model.nodes.front();
    eng::gl::VertexArray vertex_array;
    std::list<eng::gl::Buffer> vertex_buffers;
    const tinygltf::Primitive &primitive = model.meshes[node.mesh].primitives.front();
    size_t vertex_count = model.accessors[primitive.attributes.at("POSITION")].count;
    for (const auto &[_, index] : primitive.attributes)
    {
        const tinygltf::Accessor &accessor = model.accessors.at(index);
        const tinygltf::BufferView &view = model.bufferViews.at(accessor.bufferView);
        const tinygltf::Buffer &buffer = model.buffers.at(view.buffer);
        vertex_buffers.emplace_back(buffer.data.data() + view.byteOffset, view.byteLength);
        vertex_array.add_buffer(index, vertex_buffers.back(), accessor, model);
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(.32f, .85f, .58f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();
        vertex_array.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}