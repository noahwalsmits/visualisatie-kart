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
	/*Creates a model with a map of integers corresponding to animation filepaths.
	These integers can then be used to play their corresponding animations.*/
	AnimatedModel(std::string const& modelPath, const std::map<int, std::string>& animationPaths, glm::vec3 startPosition, int startAnimation);

	/*Creates a model with a single animation.*/
	AnimatedModel(std::string const& modelPath, glm::vec3 startPosition);

	void draw(Shader& shader);
	void update(float deltaTime);
	void playAnimation(int animationKey, bool loopAnimation = true);
	void setAnimationSpeed(float speed);

private:
	Animator animator;
	std::map<int, Animation> animations; //TODO allow the key to be a generic type
};