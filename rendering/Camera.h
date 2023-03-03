#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	glm::mat4 getView();
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);

private:
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
};

