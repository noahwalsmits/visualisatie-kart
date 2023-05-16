#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3& targetPosition) : target(targetPosition)
{
	updateViewingAngle();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(this->position, this->target, this->worldUp);
}

void Camera::mouseMoved(double deltaX, double deltaY)
{
	this->yaw -= deltaX * this->MOUSE_SENSITIVITY;
	this->yaw = remainderf(this->yaw, 360.0f);

	this->pitch -= deltaY * this->MOUSE_SENSITIVITY;
	if (pitch > this->MAX_PITCH)
	{
		pitch = this->MAX_PITCH;
	}
	if (pitch < this->MIN_PITCH)
	{
		pitch = this->MIN_PITCH;
	}

	this->updateViewingAngle();
}

void Camera::pollMovementKeys(float deltaTime, GLFWwindow* window)
{
	
}

void Camera::updateViewingAngle()
{
	this->position.x = this->target.x + this->distance * cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
	this->position.y = this->target.y + this->distance * sin(glm::radians(this->pitch));
	this->position.z = this->target.z + this->distance * cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
}
