#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "../model.h"
#include "bone.h"

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
public:
	Animation() = default;
	Animation(const std::string& animationPath, Model* model);
	~Animation()
	{

	}

	Bone* findBone(const std::string& name);

	inline float getTicksPerSecond() { return this->ticksPerSecond; }
	inline float getDuration() { return this->duration; }
	inline const AssimpNodeData& getRootNode() { return this->rootNode; }
	inline const std::map<std::string, Mesh::BoneInfo>& getBoneIDMap() { return this->boneInfoMap; }

private:
	float duration;
	int ticksPerSecond;
	std::vector<Bone> bones;
	AssimpNodeData rootNode;
	std::map<std::string, Mesh::BoneInfo> boneInfoMap;

	void readMissingBones(const aiAnimation* animation, Model& model);
	void readHeirarchyData(AssimpNodeData& dest, const aiNode* src);
};
