#pragma once
#include "../rendering/animation/AnimatedModel.h"

class PlayerCharacter
{
public:
	PlayerCharacter()
	{

	}

	void update(float deltaTime);

private:
	//AnimatedModel carModel;
	AnimatedModel driverModel = AnimatedModel("assets/animated_yoshi/yoshi.dae",
		{ "assets/animated_yoshi/yoshiLeft.dae", "assets/animated_yoshi/yoshiRight.dae" });
};

