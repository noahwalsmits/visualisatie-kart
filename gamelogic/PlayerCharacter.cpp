#include "PlayerCharacter.h"
#include <algorithm>

//TODO change to use callbacks instead
void PlayerCharacter::update(float deltaTime, GLFWwindow* window)
{
	//driving
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //forwards
	{
		if (this->speed < 0.0f) //brake
		{
			this->speed += BRAKING_DECCELERATION * deltaTime;
			this->speed = std::min(this->speed, 0.0f);
		}
		else //drive forwards
		{
			this->speed += FORWARD_ACCELERATION * deltaTime;
			this->speed = std::min(this->speed, MAX_FORWARD_SPEED);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //backwards
	{
		if (this->speed > 0.0f) //brake
		{
			this->speed -= BRAKING_DECCELERATION * deltaTime;
			this->speed = std::max(this->speed, 0.0f);
		}
		else //drive backwards
		{
			this->speed += REVERSE_ACCELERATION * deltaTime;
			this->speed = std::max(this->speed, MAX_REVERSE_SPEED);
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)) //idle
	{
		if (this->speed > 0.0f) //slow to a stop
		{
			this->speed -= IDLE_DECCELERATION * deltaTime;
			this->speed = std::max(this->speed, 0.0f);
		}
		if (this->speed < 0.0f) //slow to a stop
		{
			this->speed += IDLE_DECCELERATION * deltaTime;
			this->speed = std::min(this->speed, 0.0f);
		}
	}

	//steering
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //left
	{
		if (this->speed < 0 || this->speed > 0) //can't turn if you're standing still
		{
			this->rotation += std::min(TURNING_RATE * deltaTime, (TURNING_RATE * deltaTime) / abs(this->speed));
			this->rotation = remainderf(this->rotation, 360.0f);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //right
	{
		if (this->speed < 0 || this->speed > 0) //can't turn if you're standing still
		{
			this->rotation -= std::min(TURNING_RATE * deltaTime, (TURNING_RATE * deltaTime) / abs(this->speed));
			this->rotation = remainderf(this->rotation, 360.0f);
		}
	}

	//update model position
	this->driverModel.position.z += cos(glm::radians(this->rotation)) * this->speed;
	this->driverModel.position.x += sin(glm::radians(this->rotation)) * this->speed;
	this->driverModel.rotationYaw = this->rotation; //make models use rotation as reference
	//TODO displace all models from a single shared position in PlayerCharacter.draw()
}
