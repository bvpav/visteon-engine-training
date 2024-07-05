#include <iostream>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "eng/gl/shader.hh"
#include "eng/gl/program.hh"
#include "eng/mesh/vertex.hh"
#include "eng/gl/buffer.hh"

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

    // std::array vertices = {
    //     eng::mesh::Vertex{-0.5f, -0.5f, 0.0f, // positions[0]​
    //                        0.0f,  0.0f, 1.0f, // normals[0]​
    //                        0.25f, 0.25f},     // texture coordinates[0]​
    //     eng::mesh::Vertex{ 0.5f, -0.5f, 0.0f, // positions[1]​
    //                        0.0f,  0.0f, 1.0f, // normals[1]​
    //                        0.75f, 0.25f},     // texture coordinates[1]​
    //     eng::mesh::Vertex{ 0.0f,  0.5f, 0.0f, // positions[2]​
    //                        0.0f,  0.0f, 1.0f, // normals[2]​
    //                        0.5f,  0.75f},     // texture coordinates[2]​
    // };
    // eng::gl::Buffer vertex_buffer(vertices);

    GLfloat positions[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };
    GLfloat normals[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };
    GLfloat uvs[] = {
        0.25f, 0.25f, // texture coordinates[0]​
        0.75f, 0.25f, // texture coordinates[1]​
        0.5f, 0.75f // texture coordinates[2]​
    };

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof positions, positions, GL_STATIC_DRAW);

    GLuint normal_buffer;
    glGenBuffers(1, &normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof normals, normals, GL_STATIC_DRAW);

    GLuint uv_buffer;
    glGenBuffers(1, &uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof uvs, uvs, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), nullptr);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof (GLfloat), nullptr);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(.32f, .85f, .58f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}