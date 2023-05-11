#pragma once
#include "../model.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "Animator.h"
#include "Animation.h"
#include <glad/glad.h>

class AnimatedModel : public Model
{
public:
	AnimatedModel(std::string const& modelPath, const std::map<int, std::string>& animationPaths, glm::vec3 startPosition, int startAnimation)
		: Model(modelPath, startPosition)
	{
		//all animations must be exported as seperate .dae files because Blender :(
		for (const auto& entry : animationPaths)
		{
			this->animations[entry.first] = Animation(entry.second, this);
		}
		this->playAnimation(startAnimation);
	}

	/*Creates a model with a single animation.*/
	AnimatedModel(std::string const& modelPath, glm::vec3 startPosition) 
		: AnimatedModel(modelPath, { {0, modelPath} }, startPosition, 0)
	{

	}

	void draw(Shader& shader);
	void update(float deltaTime);
	void playAnimation(int animationKey, bool loopAnimation = true);

private:
	Animator animator;
	std::map<int, Animation> animations; //TODO allow the key to be a generic type
};