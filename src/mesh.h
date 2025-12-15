#ifndef MESH_H
#define MESH_H
#include <string>
#include "camera.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};
class Mesh
{
    public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    std::vector <Texture> textures;
    GLuint VAO, VBO, EBO;
    void setupBuffors();
    Mesh(std::vector <Vertex>& verts, std::vector <GLuint> &inds, std::vector <Texture> &texts);
    void draw(Shaders* shader, Camera* camera, glm::mat4 model, glm::vec3 defaultColor);

};

#endif