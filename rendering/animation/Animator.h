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

	void UpdateAnimation(float dt);
	void PlayAnimation(Animation pAnimation, bool loopAnimation = true, float animationSpeed = 1.0f);
	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetFinalBoneMatrices() { return m_FinalBoneMatrices; }

private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	Animation m_CurrentAnimation;
	float m_CurrentTime;
	bool looping = true;
	bool finished = true;
};