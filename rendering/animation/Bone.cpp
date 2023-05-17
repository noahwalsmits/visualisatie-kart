#include "Bone.h"

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
	: name(name), boneID(ID), localTransform(1.0f)
{
	this->numPositions = channel->mNumPositionKeys;

	for (int positionIndex = 0; positionIndex < this->numPositions; ++positionIndex)
	{
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		float timeStamp = channel->mPositionKeys[positionIndex].mTime;
		KeyPosition data;
		data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
		data.timeStamp = timeStamp;
		this->positions.push_back(data);
	}

	this->numRotations = channel->mNumRotationKeys;
	for (int rotationIndex = 0; rotationIndex < this->numRotations; ++rotationIndex)
	{
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
		KeyRotation data;
		data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
		data.timeStamp = timeStamp;
		this->rotations.push_back(data);
	}

	this->numScalings = channel->mNumScalingKeys;
	for (int keyIndex = 0; keyIndex < this->numScalings; ++keyIndex)
	{
		aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
		float timeStamp = channel->mScalingKeys[keyIndex].mTime;
		KeyScale data;
		data.scale = AssimpGLMHelpers::GetGLMVec(scale);
		data.timeStamp = timeStamp;
		this->scales.push_back(data);
	}
}

void Bone::update(float animationTime)
{
	glm::mat4 translation = interpolatePosition(animationTime);
	glm::mat4 rotation = interpolateRotation(animationTime);
	glm::mat4 scale = interpolateScaling(animationTime);
	this->localTransform = translation * rotation * scale;
}

int Bone::getPositionIndex(float animationTime)
{
	for (int index = 0; index < this->numPositions - 1; ++index)
	{
		if (animationTime < this->positions[index + 1].timeStamp)
			return index;
	}
	assert(false);
}

int Bone::getRotationIndex(float animationTime)
{
	for (int index = 0; index < this->numRotations - 1; ++index)
	{
		if (animationTime < this->rotations[index + 1].timeStamp)
			return index;
	}
	assert(false);
}

int Bone::getScaleIndex(float animationTime)
{
	for (int index = 0; index < this->numScalings - 1; ++index)
	{
		if (animationTime < this->scales[index + 1].timeStamp)
			return index;
	}
	assert(false);
}

float Bone::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

glm::mat4 Bone::interpolatePosition(float animationTime)
{
	if (this->numPositions == 1)
		return glm::translate(glm::mat4(1.0f), this->positions[0].position);

	int p0Index = getPositionIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = getScaleFactor(this->positions[p0Index].timeStamp, this->positions[p1Index].timeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(this->positions[p0Index].position, this->positions[p1Index].position, scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::interpolateRotation(float animationTime)
{
	if (this->numRotations == 1)
	{
		auto rotation = glm::normalize(this->rotations[0].orientation);
		return glm::toMat4(rotation);
	}

	int p0Index = getRotationIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = getScaleFactor(this->rotations[p0Index].timeStamp, this->rotations[p1Index].timeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(this->rotations[p0Index].orientation, this->rotations[p1Index].orientation, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);
}

glm::mat4 Bone::interpolateScaling(float animationTime)
{
	if (this->numScalings == 1)
		return glm::scale(glm::mat4(1.0f), this->scales[0].scale);

	int p0Index = getScaleIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = getScaleFactor(this->scales[p0Index].timeStamp, this->scales[p1Index].timeStamp, animationTime);
	glm::vec3 finalScale = glm::mix(this->scales[p0Index].scale, this->scales[p1Index].scale, scaleFactor);
	return glm::scale(glm::mat4(1.0f), finalScale);
}
