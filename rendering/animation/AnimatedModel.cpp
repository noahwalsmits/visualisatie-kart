#include "AnimatedModel.h"

AnimatedModel::AnimatedModel(std::string const& modelPath, const std::map<int, std::string>& animationPaths, glm::vec3 startPosition, int startAnimation)
	: Model(modelPath, startPosition)
{
	//all animations must be exported as seperate .dae files because Blender :(
	for (const auto& entry : animationPaths)
	{
		this->animations[entry.first] = Animation(entry.second, this);
	}
	this->playAnimation(startAnimation);
}

AnimatedModel::AnimatedModel(std::string const& modelPath, glm::vec3 startPosition)
	: AnimatedModel(modelPath, { {0, modelPath} }, startPosition, 0)
{

}

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

void AnimatedModel::setAnimationSpeed(float speed)
{
	this->animator.speed = speed;
}
