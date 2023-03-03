#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
}

glm::mat4 Camera::getView()
{
	//return glm::lookAt(this->position, this->target, this->up); //orbits the target
	return glm::lookAt(this->position, this->position + this->front, this->up);
}
