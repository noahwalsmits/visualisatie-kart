#pragma once
#include "../model.h"
#include <vector>
#include <glm/glm.hpp>
#include "Animator.h"
#include "Animation.h"
#include <glad/glad.h>

class AnimatedModel : public Model
{
public:
	//TODO value key template to add models to a map
	AnimatedModel(std::string const& modelPath, std::initializer_list<std::string> animationPaths, glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f))
		: Model(modelPath, startPosition)
	{
		//all animations must be exported as seperate .dae files because Blender :(
		for (std::string const& animationPath : animationPaths)
		{
			this->animations.push_back(Animation(animationPath, this));
		}
		this->animator.PlayAnimation(this->animations[0]);
	}

	void draw(Shader& shader);
	void update(float deltaTime);
	void playAnimation(int animationIndex, bool loopAnimation = true);

private:
	Animator animator;
	std::vector<Animation> animations;
};