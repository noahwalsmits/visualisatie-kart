#include "Animator.h"

Animator::Animator()
{
	this->currentTime = 0.0;
	this->finished = true; //don't update because we start without an animation loaded

	this->finalBoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
	{
		this->finalBoneMatrices.push_back(glm::mat4(1.0f));
	}
}

void Animator::UpdateAnimation(float deltaTime)
{
	if (this->finished) //TODO support no current animation and check for it here
	{
		return;
	}

	this->currentTime += this->speed * this->currentAnimation.getTicksPerSecond() * deltaTime;
	if (this->looping) //restart animation
	{
		this->currentTime = fmod(this->currentTime, this->currentAnimation.getDuration());
		if (this->currentTime < 0)
		{
			this->currentTime = this->currentAnimation.getDuration() + this->currentTime;
		}
	}
	else if (this->currentTime >= this->currentAnimation.getDuration()) //end animation
	{
		this->finished = true;
		this->currentTime = std::nextafter(this->currentAnimation.getDuration(), 0.0f); //set it to a value just under the duration
	}
	else if (this->currentTime < 0.0f) //end reverse animation
	{
		this->finished = true;
		this->currentTime = 0.0f;
	}
	CalculateBoneTransform(&this->currentAnimation.getRootNode(), glm::mat4(1.0f));
}

void Animator::PlayAnimation(Animation animation, bool loopAnimation, float animationSpeed)
{
	//TODO pointer doesn't work here because the AnimatedModel is copied when added to the vector
	this->currentAnimation = animation;
	this->currentTime = 0.0f;
	this->speed = animationSpeed;
	this->looping = loopAnimation;
	this->finished = false;
	if (this->speed < 0) //reverse animations start at the end and count down from there
	{
		this->currentTime = std::nextafter(this->currentAnimation.getDuration(), 0.0f); //set it to a value just under the duration
	}
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* Bone = this->currentAnimation.findBone(nodeName);

	if (Bone)
	{
		Bone->Update(this->currentTime);
		nodeTransform = Bone->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = this->currentAnimation.getBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		this->finalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
	{
		CalculateBoneTransform(&node->children[i], globalTransformation);
	}
}