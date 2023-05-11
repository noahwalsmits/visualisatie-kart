#include "AnimatedModel.h"

void AnimatedModel::draw(Shader& shader)
{
	std::vector<glm::mat4> transforms = this->animator.GetFinalBoneMatrices();
	for (int i = 0; i < transforms.size(); i++)
	{
		shader.setMat4("finalBoneMatrices[" + std::to_string(i) + "]", transforms[i]);
	}
	Model::draw(shader);
}

void AnimatedModel::update(float deltaTime)
{
	this->animator.UpdateAnimation(deltaTime);
}

void AnimatedModel::playAnimation(int animationKey, bool loopAnimation)
{
	assert(this->animations.count(animationKey) == 1);
	this->animator.PlayAnimation(this->animations[animationKey], loopAnimation);
}
