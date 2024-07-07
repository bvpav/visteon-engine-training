#include <iostream>
#include <array>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "eng/gl/shader.hh"
#include "eng/gl/program.hh"
#include "eng/mesh/vertex.hh"
#include "eng/gl/buffer.hh"
#include "eng/gl/vertexarray.hh"

void handle_key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto handler = static_cast<std::function<void(int, int)> *>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        (*handler)(key, action);
    }
}

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

    std::array vertices = {
        eng::mesh::Vertex{-0.5f, -0.5f, 0.0f, // positions[0]
                           0.0f,  0.0f, 1.0f, // normals[0]
                           0.25f, 0.25f},     // texture coordinates[0]
        eng::mesh::Vertex{ 0.5f, -0.5f, 0.0f, // positions[1]
                           0.0f,  0.0f, 1.0f, // normals[1]
                           0.75f, 0.25f},     // texture coordinates[1]
        eng::mesh::Vertex{ 0.0f,  0.5f, 0.0f, // positions[2]
                           0.0f,  0.0f, 1.0f, // normals[2]
                           0.5f,  0.75f},     // texture coordinates[2]
    };
    eng::gl::Buffer vertex_buffer(vertices);

    eng::gl::VertexArray vertex_array1 = eng::gl::VertexArray::from<eng::mesh::Vertex>(vertex_buffer);
    eng::gl::VertexArray vertex_array2 = eng::gl::VertexArray::from<eng::mesh::WeirdVertex>(vertex_buffer);
    eng::gl::VertexArray *current_vertex_array = &vertex_array1;

    std::function handler = [&](int key, int action)
    {
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
            current_vertex_array = current_vertex_array == &vertex_array1 ? &vertex_array2 : &vertex_array1;
    };
    glfwSetWindowUserPointer(window, &handler);
    glfwSetKeyCallback(window, handle_key);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(.32f, .85f, .58f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();
        vertex_buffer.bind();
        current_vertex_array->bind();
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}