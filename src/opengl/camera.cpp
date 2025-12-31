#include "camera.h"

Camera::Camera(glm::vec3 position)
{
    this->position = position;
}

void Camera::updateMat(float FOVdeg, float nearPlane, float farPlane, int width, int height)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    view = glm::lookAt(position, position+orientation, up);
    proj = glm::perspective(glm::radians(FOVdeg), (float)(width)/float(height), nearPlane, farPlane);

	camMatrix = proj*view;
}

void Camera::matrix(Shaders* shader, const char* uniform )
{

	 glUniformMatrix4fv(glGetUniformLocation(shader->getID(), uniform), 1, GL_FALSE, glm::value_ptr(camMatrix));
}

void Camera::inputs(GLFWwindow* window, int width, int height)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += speed*orientation;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= speed*glm::normalize(glm::cross(orientation, up));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= speed*orientation;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += speed*glm::normalize(glm::cross(orientation, up));


    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position += speed*up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        position -= speed*up;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

		if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			orientation = newOrientation;
		}

		orientation = glm::rotate(orientation, glm::radians(-rotY), up);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}

}