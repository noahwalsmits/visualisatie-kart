#include "Animator.h"

Animator::Animator()
{
	m_CurrentTime = 0.0;
	this->finished = true; //don't update because we start without an animation loaded

	m_FinalBoneMatrices.reserve(100);

	for (int i = 0; i < 100; i++)
	{
		m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}
}

void Animator::UpdateAnimation(float dt)
{
	if (this->finished) //TODO support no current animation and check for it here
	{
		return;
	}

	m_CurrentTime += this->speed * m_CurrentAnimation.getTicksPerSecond() * dt;
	if (this->looping) //restart animation
	{
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation.getDuration());
		if (m_CurrentTime < 0)
		{
			m_CurrentTime = m_CurrentAnimation.getDuration() + m_CurrentTime;
		}
	}
	else if (m_CurrentTime >= m_CurrentAnimation.getDuration()) //end animation
	{
		this->finished = true;
		m_CurrentTime = std::nextafter(m_CurrentAnimation.getDuration(), 0.0f); //set it to a value just under the duration
	}
	else if (m_CurrentTime < 0.0f) //end reverse animation
	{
		this->finished = true;
		m_CurrentTime = 0.0f;
	}
	CalculateBoneTransform(&m_CurrentAnimation.getRootNode(), glm::mat4(1.0f));
}

void Animator::PlayAnimation(Animation pAnimation, bool loopAnimation, float animationSpeed)
{
	//TODO pointer doesn't work here because the AnimatedModel is copied when added to the vector
	m_CurrentAnimation = pAnimation;
	m_CurrentTime = 0.0f;
	this->speed = animationSpeed;
	this->looping = loopAnimation;
	this->finished = false;
	if (this->speed < 0) //reverse animations start at the end and count down from there
	{
		m_CurrentTime = std::nextafter(m_CurrentAnimation.getDuration(), 0.0f); //set it to a value just under the duration
	}
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->name;
	glm::mat4 nodeTransform = node->transformation;

	Bone* Bone = m_CurrentAnimation.findBone(nodeName);

	if (Bone)
	{
		Bone->Update(m_CurrentTime);
		nodeTransform = Bone->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = m_CurrentAnimation.getBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		m_FinalBoneMatrices[index] = globalTransformation * offset;
	}

	for (int i = 0; i < node->childrenCount; i++)
	{
		CalculateBoneTransform(&node->children[i], globalTransformation);
	}
}