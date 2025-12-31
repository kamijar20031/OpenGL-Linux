#include "shaders.h"
Shaders::Shaders(const std::string& vert, const std::string& frag,bool debugging)    
{
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    std::string src = loadShaderSource(vert);
    const char* srcVert = src.c_str();
    glShaderSource(vertShader, 1, &srcVert, nullptr);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    src = loadShaderSource(frag);
    const char* srcFrag = src.c_str();
    glShaderSource(fragShader, 1, &srcFrag, nullptr);
    glCompileShader(fragShader);

    if (debugging)
    {
        int bufflen;
        std::cout << "Fragment Shader:" << std::endl;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &bufflen);
        GLchar* log_string = new char[bufflen + 1];
        glGetShaderInfoLog(fragShader, bufflen, 0, log_string);
        std::cout << log_string << std::endl << std::endl;

        std::cout << "Vertex Shader:" << std::endl;
        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &bufflen);
        glGetShaderInfoLog(vertShader, bufflen, 0, log_string);
        std::cout << log_string;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

std::string Shaders::loadShaderSource(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file) throw std::runtime_error("Cannot open file: " + filepath);

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shaders::useShader()
{
    glUseProgram(shaderProgram);
}
void Shaders::deleteShader()
{
    glDeleteProgram(shaderProgram);
}