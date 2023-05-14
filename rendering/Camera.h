#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

/*A camera that orbits a target position.*/
class Camera
{
public:
	Camera(glm::vec3& targetPosition);
	glm::mat4 getViewMatrix();
	void mouseMoved(double deltaX, double deltaY);
	void pollMovementKeys(float deltaTime, GLFWwindow* window);

	float getYaw() { return this->yaw; }
	void setYaw(float yaw) { this->yaw = yaw; }
	float getPitch() { return this->pitch; }
	void setPitch(float pitch) { this->pitch = pitch; }

private:
	static constexpr float MOVEMENT_SPEED = 3.0f;
	static constexpr float MOUSE_SENSITIVITY = 0.1f;
	static constexpr float MIN_PITCH = -10.0f;
	static constexpr float MAX_PITCH = 89.0f;

	const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3& target; //reference to the position the camera is looking at
	glm::vec3 position; //the current position of the camera
	float distance = 2.0f; //distance between the camera and target positions
	float yaw = -90.0f;
	float pitch = 0.0f;

	/*Updates the position of the camera based on its yaw and pitch rotations.*/
	void updateViewingAngle();
};
