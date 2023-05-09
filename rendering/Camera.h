#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	glm::mat4 getView();
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f); //TODO stay close to the player
	glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f); //TODO target the player

	void mouseMoved(double deltaX, double deltaY);

private:
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::vec3(0.0f, 0.0f, -1.0f);

	float mouseSensitivity = 0.1f;
	float yaw = -90.0f;
	float pitch = 0.0f;
};

