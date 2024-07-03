#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <optional>
#include <memory>

struct Shader
{
    static std::optional<std::unique_ptr<Shader>> from_src(const GLchar *src, GLenum type)
    {
        GLuint id = glCreateShader(type);
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);
        GLint success;
        GLchar info_log[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, sizeof info_log, nullptr, info_log);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                      << info_log << '\n';
            return std::nullopt;
        }
        return std::unique_ptr<Shader>(new Shader(id));
    }

    [[nodiscard]] GLuint id() const { return m_id; }

    ~Shader()
    {
        glDeleteShader(m_id);
    }

private:
    GLuint m_id;

    explicit Shader(GLuint id) : m_id(id) {}
};

struct Program
{
    static std::optional<std::unique_ptr<Program>> from_shaders(const Shader &vertex_shader, const Shader &fragment_shader)
    {
        GLuint id = glCreateProgram();
        glAttachShader(id, vertex_shader.id());
        glAttachShader(id, fragment_shader.id());
        glLinkProgram(id);
        GLint success;
        GLchar info_log[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, sizeof info_log, nullptr, info_log);
            std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n"
                      << info_log << '\n';
            return std::nullopt;
        }
        return std::unique_ptr<Program>(new Program(id));
    }

    void use() const
    {
        glUseProgram(m_id);
    }

    ~Program()
    {
        glDeleteProgram(m_id);
    }

private:
    GLuint m_id;

    explicit Program(GLuint id) : m_id(id) {}
};

struct VertexBuffer
{
    VertexBuffer(const float *data, GLsizeiptr size, GLenum usage)
        : m_id(0)
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
    }

    ~VertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    [[nodiscard]] GLuint id() const { return m_id; }

private:
    GLuint m_id;
};

struct VertexArray
{
    VertexArray()
        : m_id(0)
    {
        glGenVertexArrays(1, &m_id);
    }

    ~VertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void bind() const
    {
        glBindVertexArray(m_id);
    }

    [[nodiscard]] GLuint id() const { return m_id; }

private:
    GLuint m_id;
};

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

    const char *vertex_shader_src = R"(
        #version 330 core
        layout(location = 0) in vec3 position;
        void main()
        {
            gl_Position = vec4(position, 1.0);
        }
    )";
    std::unique_ptr<Shader> vertex_shader = Shader::from_src(vertex_shader_src, GL_VERTEX_SHADER).value();

    const char *fragment_shader_src = R"(
        #version 330 core
        out vec4 color;
        void main()
        {
            color = vec4(0.77, 0.0, 0.75, 1.0);
        }
    )";
    std::unique_ptr<Shader> fragment_shader = Shader::from_src(fragment_shader_src, GL_FRAGMENT_SHADER).value();

    std::unique_ptr<Program> program = Program::from_shaders(*vertex_shader, *fragment_shader).value();

    float vertices[] = {
        -0.5f, -0.5f,
        +0.0f, +0.5f,
        +0.5f, -0.5f
    };

    VertexArray vao;
    vao.bind();
    VertexBuffer vbo(vertices, sizeof vertices, GL_STATIC_DRAW);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(.32f, .85f, .58f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        program->use();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, sizeof vertices / sizeof *vertices / 2);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}