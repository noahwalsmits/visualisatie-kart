#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Animation.h"

class Animator
{
public:
	float speed = 1.0f; //speed multiplier applied to the animation, negative values will cause it to play in reverse

	Animator();

	void updateAnimation(float deltaTime);
	void playAnimation(Animation animation, bool loopAnimation = true, float animationSpeed = 1.0f);
	void calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> getFinalBoneMatrices() { return finalBoneMatrices; }

private:
	std::vector<glm::mat4> finalBoneMatrices;
	Animation currentAnimation;
	float currentTime;
	bool looping = true;
	bool finished = true;
};