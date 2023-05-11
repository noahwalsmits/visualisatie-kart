#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Animation.h"

class Animator
{
public:
	float speed = 1.0f; //speed multiplier applied to the animation, negative values will cause it to play in reverse

	Animator()
	{
		m_CurrentTime = 0.0;
		this->finished = true; //don't update because we start without an animation loaded

		m_FinalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; i++)
		{
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
		}
	}

	void UpdateAnimation(float dt)
	{
		if (this->finished) //TODO support no current animation and check for it here
		{
			return;
		}

		m_CurrentTime += this->speed * m_CurrentAnimation.GetTicksPerSecond() * dt;
		if (this->looping) //restart animation
		{
			m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation.GetDuration());
			if (m_CurrentTime < 0)
			{
				m_CurrentTime = m_CurrentAnimation.GetDuration() + m_CurrentTime;
			}
		}
		else if (m_CurrentTime >= m_CurrentAnimation.GetDuration()) //end animation
		{
			this->finished = true;
			m_CurrentTime = std::nextafter(m_CurrentAnimation.GetDuration(), 0.0f); //set it to a value just under the duration
		}
		else if (m_CurrentTime < 0.0f) //end reverse animation
		{
			this->finished = true;
			m_CurrentTime = 0.0f;
		}
		CalculateBoneTransform(&m_CurrentAnimation.GetRootNode(), glm::mat4(1.0f));
	}

	void PlayAnimation(Animation pAnimation, bool loopAnimation = true, float animationSpeed = 1.0f)
	{
		//TODO pointer doesn't work here because the AnimatedModel is copied when added to the vector
		m_CurrentAnimation = pAnimation;
		m_CurrentTime = 0.0f;
		this->speed = animationSpeed;
		this->looping = loopAnimation;
		this->finished = false;
		if (this->speed < 0) //reverse animations start at the end and count down from there
		{
			m_CurrentTime = std::nextafter(m_CurrentAnimation.GetDuration(), 0.0f); //set it to a value just under the duration
		}
	}

	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
	{
		std::string nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		Bone* Bone = m_CurrentAnimation.FindBone(nodeName);

		if (Bone)
		{
			Bone->Update(m_CurrentTime);
			nodeTransform = Bone->GetLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		auto boneInfoMap = m_CurrentAnimation.GetBoneIDMap();
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

	std::vector<glm::mat4> GetFinalBoneMatrices()
	{
		return m_FinalBoneMatrices;
	}

private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	Animation m_CurrentAnimation;
	float m_CurrentTime;
	bool looping = true;
	bool finished = true;
};