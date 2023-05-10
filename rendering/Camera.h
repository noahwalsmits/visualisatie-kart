#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	glm::mat4 getViewMatrix();
	void mouseMoved(double deltaX, double deltaY);
	void pollMovementKeys(float deltaTime, GLFWwindow* window);

private:
	static constexpr float MOVEMENT_SPEED = 3.0f;
	static constexpr float MOUSE_SENSITIVITY = 0.1f;

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f); //TODO stay close to the player
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f); //TODO target the player

	float yaw = -90.0f;
	float pitch = 0.0f;

	void updateViewingAngle();
};
