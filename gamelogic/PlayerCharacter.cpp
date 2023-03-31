#include "PlayerCharacter.h"

void PlayerCharacter::update(float deltaTime, GLFWwindow* window)
{
	//TODO update position values
	float speed = 2.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //up
	{
		this->driverModel.position.z -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //down
	{
		this->driverModel.position.z += speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //left
	{
		this->driverModel.position.x -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //right
	{
		this->driverModel.position.x += speed;
	}
}
