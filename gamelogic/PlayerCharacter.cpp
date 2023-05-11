#include "PlayerCharacter.h"
#include <algorithm>

//TODO:
//base animationstates
//refactor code

void PlayerCharacter::registerModels(std::vector<Model*>& staticModels, std::vector<AnimatedModel*>& animatedModels)
{
	animatedModels.push_back(&this->carModel);
	animatedModels.push_back(&this->driverModel);
}

void PlayerCharacter::unregisterModels(std::vector<Model*>& staticModels, std::vector<AnimatedModel*>& animatedModels)
{
	auto carPosition = std::find(animatedModels.begin(), animatedModels.end(), &this->carModel);
	if (carPosition != animatedModels.end())
	{
		animatedModels.erase(carPosition);
	}
	auto driverPosition = std::find(animatedModels.begin(), animatedModels.end(), &this->driverModel);
	if (driverPosition != animatedModels.end())
	{
		animatedModels.erase(driverPosition);
	}
}

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
		if (this->currentDriverAnimation != DriverAnimationState::steerLeft)
		{
			this->currentDriverAnimation = DriverAnimationState::steerLeft;
			this->driverModel.playAnimation((int)this->currentDriverAnimation, false);
		}
		if (this->speed < 0 || this->speed > 0) //can't turn if you're standing still
		{
			this->rotation += std::min(TURNING_RATE * deltaTime, (TURNING_RATE * deltaTime) / abs(this->speed));
			this->rotation = remainderf(this->rotation, 360.0f);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //right
	{
		if (this->currentDriverAnimation != DriverAnimationState::steerRight)
		{
			this->currentDriverAnimation = DriverAnimationState::steerRight;
			this->driverModel.playAnimation((int)this->currentDriverAnimation, false);
		}
		if (this->speed < 0 || this->speed > 0) //can't turn if you're standing still
		{
			this->rotation -= std::min(TURNING_RATE * deltaTime, (TURNING_RATE * deltaTime) / abs(this->speed));
			this->rotation = remainderf(this->rotation, 360.0f);
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)) //idle
	{
		if (this->currentDriverAnimation != DriverAnimationState::steerNeutral)
		{
			this->currentDriverAnimation = DriverAnimationState::steerNeutral;
			this->driverModel.playAnimation((int)this->currentDriverAnimation, true);
		}
	}

	//TODO change current animation based on speed

	//update models
	this->driverModel.position.z += cos(glm::radians(this->rotation)) * this->speed;
	this->driverModel.position.x += sin(glm::radians(this->rotation)) * this->speed;
	this->driverModel.rotationYaw = this->rotation;

	this->carModel.position.z += cos(glm::radians(this->rotation)) * this->speed;
	this->carModel.position.x += sin(glm::radians(this->rotation)) * this->speed;
	this->carModel.rotationYaw = this->rotation;
	//TODO make models use rotation as reference
	//TODO displace all models from a single shared position

	//TODO make better use of the character position
	this->position.z += cos(glm::radians(this->rotation)) * this->speed;
	this->position.x += sin(glm::radians(this->rotation)) * this->speed;
}
