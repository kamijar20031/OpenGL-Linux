#include "mesh.h"

Mesh::Mesh(std::vector <Vertex>& verts, std::vector <GLuint> &inds, std::vector <Texture> &texts) : vertices(verts), indices(inds), textures(texts)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(9*sizeof(float)));
    
    glBindVertexArray(0);
}
void Mesh::draw(Shaders* shader, Camera* camera, glm::mat4 matrix, glm::vec3 defaultColor)
{
    shader->useShader();
    glBindVertexArray(VAO);

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i=0; i<textures.size(); i++)
    {
        std::string num;
        std::string type = textures[i].type;
        if (type=="diffuse")
        {
            num = std::to_string(numDiffuse);
            numDiffuse++;
        }
        else if (type=="specular")
        {
            num = std::to_string(numSpecular);
            numSpecular++;
            
        }
        textures[i].texUnit(shader, (type+num).c_str(),i);
        textures[i].bind();
    }

    glUniform3f(glGetUniformLocation(shader->getID(), "generatedColor"), defaultColor.x, defaultColor.y, defaultColor.z);
    glUniform3f(glGetUniformLocation(shader->getID(), "camPos"), camera->position.x, camera->position.y, camera->position.z);
    camera->matrix(shader, "camMatrix");
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(matrix));
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}