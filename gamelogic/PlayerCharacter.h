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
};

