#pragma once
#include "../rendering/animation/AnimatedModel.h"
#include <GLFW/glfw3.h>

class PlayerCharacter
{
public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	PlayerCharacter()
	{
		this->driverModel.offset = glm::vec3(0.0f, 0.35f, -0.2f);
	}

	void update(float deltaTime, GLFWwindow* window);
	void registerModels(std::vector<Model*>& staticModels, std::vector<AnimatedModel*>& animatedModels);
	void unregisterModels(std::vector<Model*>& staticModels, std::vector<AnimatedModel*>& animatedModels);

private:
	//animation state values match with animation key on the driver model
	enum class DriverAnimationState { steerLeft, steerNeutral, steerRight };
	DriverAnimationState currentDriverAnimation = DriverAnimationState::steerNeutral;

	AnimatedModel driverModel = AnimatedModel("assets/animated_yoshi/yoshi.dae",
		{ 
			{ (int)DriverAnimationState::steerNeutral, "assets/animated_yoshi/yoshiNeutral.dae"}, 
			{ (int)DriverAnimationState::steerLeft, "assets/animated_yoshi/yoshiLeft.dae" },
			{ (int)DriverAnimationState::steerRight, "assets/animated_yoshi/yoshiRight.dae" }
		});
	Model carModel = Model("assets/egg1/egg1.obj");

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
};

