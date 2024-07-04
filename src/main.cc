#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <tiny_gltf.h>

#include "eng/gl/shader.hh"
#include "eng/gl/program.hh"

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
            gl_Position = vec4(a_pos.xyz, 1.0);
        }
    )";
    eng::gl::Shader vertex_shader = eng::gl::Shader::from_src(vertex_shader_src, GL_VERTEX_SHADER).value();

    const GLchar *fragment_shader_src = R"(
        #version 330 core
        out vec4 out_frag_color;

        void main()
        {
            out_frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
    )";
    eng::gl::Shader fragment_shader = eng::gl::Shader::from_src(fragment_shader_src, GL_FRAGMENT_SHADER).value();

    eng::gl::Program program = eng::gl::Program::with_shaders(vertex_shader, fragment_shader).value();

    GLfloat vertices[] = {
        -0.5f, -0.5f,
        +0.0f, +0.5f,
        +0.5f, -0.5f,
    };
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(.32f, .85f, .58f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor3f(.77f, 0.f, .75f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(+0.0f, +0.5f);
        glVertex2f(+0.5f, -0.5f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}