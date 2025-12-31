#ifndef CAMERA_H
#define CAMERA_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaders.h"

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 camMatrix = glm::mat4(1.0f);

    bool firstClick;
    float speed = 0.1f;
    float sensitivity = 100.0f;

    Camera() {}
    Camera(glm::vec3 position);

    void updateMat(float FOVdeg, float nearPlane, float farPlane, int width, int height);
    void matrix(Shaders* shader, const char* uniform);
    void inputs(GLFWwindow* window, int width, int height);
};

#endif