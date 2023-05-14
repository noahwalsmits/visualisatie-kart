#pragma once
#include "../rendering/animation/AnimatedModel.h"
#include <GLFW/glfw3.h>

class PlayerCharacter
{
public:
	PlayerCharacter(glm::vec3 startPosition) : characterPosition(startPosition)
	{
		this->driverModel.offset = glm::vec3(0.0f, 0.35f, -0.2f);
		this->carModel.playAnimation(0);
	}

	void update(float deltaTime, GLFWwindow* window);
	void registerModels(std::vector<Model*>& staticModels, std::vector<AnimatedModel*>& animatedModels);
	void unregisterModels(std::vector<Model*>& staticModels, std::vector<AnimatedModel*>& animatedModels);
	glm::vec3& getCharacterPosition() { return this->characterPosition; }
	glm::vec3& getCameraTarget() { return this->cameraTarget; }
	float getRotation() { return this->rotation; }

private:
	//animation state values match with animation key on the driver model
	enum class DriverAnimationState { steerLeft, steerNeutral, steerRight };
	DriverAnimationState currentDriverAnimation = DriverAnimationState::steerNeutral;

	AnimatedModel driverModel = AnimatedModel("assets/animated_yoshi/yoshi.dae",
		{ 
			{ (int)DriverAnimationState::steerNeutral, "assets/animated_yoshi/yoshiNeutral.dae"}, 
			{ (int)DriverAnimationState::steerLeft, "assets/animated_yoshi/yoshiLeft.dae" },
			{ (int)DriverAnimationState::steerRight, "assets/animated_yoshi/yoshiRight.dae" }
		}, this->characterPosition, (int)DriverAnimationState::steerNeutral);

	AnimatedModel carModel = AnimatedModel("assets/animated_egg1/egg1.dae", this->characterPosition);

	//speed that is added (per second) while going forwards
	static constexpr float FORWARD_ACCELERATION = 0.1f;
	//speed that is added (per second) while going backwards
	static constexpr float REVERSE_ACCELERATION = -0.05f;
	//maximum speed that can be reached while going forwards
	static constexpr float MAX_FORWARD_SPEED = 0.2f;
	//maximum speed that can be reached while going backwards
	static constexpr float MAX_REVERSE_SPEED = -0.1f;
	//speed change (per second) while reversing with positive speed or going forward with negative speed
	static constexpr float BRAKING_DECCELERATION = 0.3f;
	//speed change (per second) towards 0 while not going forward or reversing
	static constexpr float IDLE_DECCELERATION = 0.2f;
	//the turning rate (degrees per second) while the speed is between -1 and 1
	//the applied turning decreases as the speed goes up
	static constexpr float TURNING_RATE = 100.0f;

	//the current forward positional change per second
	float speed = 0.0f;
	//the current direction in degrees
	float rotation = 0.0f;
	//the current position of the character
	glm::vec3 characterPosition;
	//the offset between the character position and the camera target position
	glm::vec3 cameraTargetOffset = glm::vec3(0.0f, 0.7f, 0.0f);
	//the position the camera following the character should be targeting
	glm::vec3 cameraTarget = this->characterPosition + this->cameraTargetOffset;

	/*Plays the animation of the requested animationState if it is not already being played.*/
	void useAnimationState(DriverAnimationState animationState, bool loopAnimation);
};

