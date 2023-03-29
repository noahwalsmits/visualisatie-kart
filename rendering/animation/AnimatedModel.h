#pragma once
#include "../model.h"
#include "Animator.h"
#include "Animation.h"

class AnimatedModel : public Model
{
public:
	AnimatedModel(std::string const& path, glm::vec3 position) : Model(path, position), animation(path, this), animator(this->animation) { }
	void draw(Shader& shader);
	void update(float deltaTime);

private:
	//NOTE THE INITIALIZATION ORDER
	Animation animation; //TODO support an array of animations
	Animator animator;
};