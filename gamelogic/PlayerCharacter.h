#pragma once
#include "../rendering/animation/AnimatedModel.h"
#include <GLFW/glfw3.h>

class PlayerCharacter
{
public:
	PlayerCharacter()
	{

	}

	void update(float deltaTime, GLFWwindow* window);
	AnimatedModel& getAnimatedModels() { return this->driverModel; };

private:
	//AnimatedModel carModel;
	AnimatedModel driverModel = AnimatedModel("assets/animated_yoshi/yoshi.dae",
		{ "assets/animated_yoshi/yoshiLeft.dae", "assets/animated_yoshi/yoshiRight.dae" });

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

	//the turning rate while the speed is between -1 and 1 (degrees per second)
	//the applied turning decreases as the speed goes up
	static constexpr float TURNING_RATE = 100.0f;

	//the current forward positional change per second
	float speed = 0.0f;
	//the current direction in degrees
	float rotation = 0.0f;
};

