#include "PlayerCharacter.h"
#include <algorithm>

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
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //forwards
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
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //backwards
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
	if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)) //idle
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
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //left
	{
		this->useAnimationState(DriverAnimationState::steerLeft, false);
		if (this->speed < 0 || this->speed > 0) //can't turn if you're standing still
		{
			this->rotation += std::min(TURNING_RATE * deltaTime, (TURNING_RATE * deltaTime) / abs(this->speed));
			this->rotation = remainderf(this->rotation, 360.0f);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //right
	{
		this->useAnimationState(DriverAnimationState::steerRight, false);
		if (this->speed < 0 || this->speed > 0) //can't turn if you're standing still
		{
			this->rotation -= std::min(TURNING_RATE * deltaTime, (TURNING_RATE * deltaTime) / abs(this->speed));
			this->rotation = remainderf(this->rotation, 360.0f);
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) && (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)) //idle
	{
		this->useAnimationState(DriverAnimationState::steerNeutral, true);
	}

	//update position
	this->characterPosition.z += cos(glm::radians(this->rotation)) * this->speed;
	this->characterPosition.x += sin(glm::radians(this->rotation)) * this->speed;
	cameraTarget = this->characterPosition + this->cameraTargetOffset;

	//update models
	this->driverModel.position = this->characterPosition;
	this->driverModel.rotationYaw = this->rotation;
	this->carModel.position = this->characterPosition;
	this->carModel.rotationYaw = this->rotation;
	this->carModel.setAnimationSpeed(this->speed * 20.0f);
}

void PlayerCharacter::useAnimationState(DriverAnimationState animationState, bool loopAnimation)
{
	if (this->currentDriverAnimation != animationState)
	{
		this->currentDriverAnimation = animationState;
		this->driverModel.playAnimation((int)this->currentDriverAnimation, loopAnimation);
	}
}
