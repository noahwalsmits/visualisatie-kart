#pragma once
#include "../model.h"
#include <vector>
#include "Animator.h"
#include "Animation.h"

class AnimatedModel : public Model
{
public:
	template <class T>
	AnimatedModel(std::string const& modelPath, glm::vec3 position, std::initializer_list<T> animationPaths) : Model(modelPath, position)
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
	void playAnimation(int animationIndex);

private:
	Animator animator;
	std::vector<Animation> animations;
};