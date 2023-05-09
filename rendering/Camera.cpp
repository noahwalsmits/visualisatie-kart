#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	updateViewingAngle();
}

glm::mat4 Camera::getViewMatrix()
{
	//return glm::lookAt(this->position, this->target, this->up); //orbits the target
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::mouseMoved(double deltaX, double deltaY)
{
	this->yaw += deltaX * this->mouseSensitivity;
	this->pitch -= deltaY * this->mouseSensitivity;
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	this->updateViewingAngle();
}

void Camera::updateViewingAngle()
{
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(this->yaw) * cos(glm::radians(this->pitch)));
	this->front = glm::normalize(direction);

	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}
