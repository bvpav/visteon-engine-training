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
        #version 300 es

        layout (location = 0) in vec3 a_pos;
        layout (location = 4) in vec4 a_color;

        out vec4 v_color;

        void main()
        {
            v_color = a_color;
            gl_Position = vec4(a_pos, 1.0);
        }
    )";
    eng::gl::Shader vertex_shader = eng::gl::Shader::from_src(vertex_shader_src, GL_VERTEX_SHADER).value();

    const GLchar *fragment_shader_src = R"(
        #version 300 es

        in vec4 v_color;

        out highp vec4 out_frag_color;

        void main()
        {
            out_frag_color = v_color;
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

    // 3 vertices, rgba
    float colors[] = {
        1.f, 0.f, 0.f, 1.f, // red
        0.f, 1.f, 0.f, 1.f, // green
        0.f, 0.f, 1.f, 1.f, // blue
    };
    GLuint color_buffer;
    glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    vertex_array.bind();
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);

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