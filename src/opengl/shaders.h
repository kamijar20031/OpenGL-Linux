#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shaders
{
    GLuint shaderProgram;
public:
    Shaders() {};
    Shaders(const std::string& vert, const std::string& frag, bool debugging=false);
    std::string loadShaderSource(const std::string& filepath);
    GLuint getID() {return shaderProgram;}
    void useShader();
    void deleteShader();
};

#endif