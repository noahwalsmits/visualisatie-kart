#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL //so we can use glm/gtx
#include <glm/gtx/quaternion.hpp>
//#include <glm/gtc/quaternion.hpp> //TODO replace toMat4 to get rid of GLM_ENABLE_EXPERIMENTAL
#include <assimp/scene.h>
#include "../../AssimpGLMHelpers.h"

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class Bone
{
public:
	/*reads keyframes from aiNodeAnim*/
	Bone(const std::string& name, int ID, const aiNodeAnim* channel);

	/*interpolates  b/w positions,rotations & scaling keys based on the curretn time of
	the animation and prepares the local transformation matrix by combining all keys
	tranformations*/
	void update(float animationTime);
	glm::mat4 getLocalTransform() { return localTransform; }
	std::string getBoneName() const { return name; }
	int getBoneID() { return boneID; }

	/* Gets the current index on mKeyPositions to interpolate to based on
	the current animation time*/
	int getPositionIndex(float animationTime);
	/* Gets the current index on mKeyRotations to interpolate to based on the
	current animation time*/
	int getRotationIndex(float animationTime);
	/* Gets the current index on mKeyScalings to interpolate to based on the
	current animation time */
	int getScaleIndex(float animationTime);

private:
	std::vector<KeyPosition> positions;
	std::vector<KeyRotation> rotations;
	std::vector<KeyScale> scales;
	int numPositions;
	int numRotations;
	int numScalings;

	glm::mat4 localTransform;
	std::string name;
	int boneID;

	/* Gets normalized value for Lerp & Slerp*/
	float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	/*figures out which position keys to interpolate b/w and performs the interpolation
	and returns the translation matrix*/
	glm::mat4 interpolatePosition(float animationTime);
	/*figures out which rotations keys to interpolate b/w and performs the interpolation
	and returns the rotation matrix*/
	glm::mat4 interpolateRotation(float animationTime);
	/*figures out which scaling keys to interpolate b/w and performs the interpolation
	and returns the scale matrix*/
	glm::mat4 interpolateScaling(float animationTime);
};
