#include "shader.hh"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <glad/glad.h>

namespace eng::gl {

std::optional<Shader> Shader::from_src(const GLchar *src, GLenum type)
{
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    GLint has_compiled_successfully;
    glGetShaderiv(id, GL_COMPILE_STATUS, &has_compiled_successfully);
    if (!has_compiled_successfully)
    {
        GLint log_length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<GLchar> info_log(log_length);
        glGetShaderInfoLog(id, log_length, nullptr, info_log.data());
        std::cerr << "error compiling shader: \n"
                  << info_log.data() << '\n';
        return std::nullopt;
    }
    return std::move(Shader(id));
}

std::optional<Shader> Shader::from_file(const std::filesystem::path &path, GLenum type)
{
    std::ifstream file(path);
    std::stringstream stream;
    stream << file.rdbuf();
    const std::string src = stream.str();
    return from_src(src.c_str(), type);

}

std::optional<Shader> Shader::default_vertex()
{
    const GLchar *default_vertex_src = R"(
        #version 300 es
        layout (location = 0) in vec3 a_pos;

        void main()
        {
            gl_Position = vec4(a_pos, 1.0);
        }
    )";
    return from_src(default_vertex_src, GL_VERTEX_SHADER);
}

std::optional<Shader> Shader::default_fragment()
{
    const GLchar *default_fragment_src = R"(
        #version 300 es
        out highp vec4 out_frag_color;

        void main()
        {
            out_frag_color = vec4(0.77f, 0.0f, 0.75f, 1.0f);
        }
    )";
    return from_src(default_fragment_src, GL_FRAGMENT_SHADER);
}

}