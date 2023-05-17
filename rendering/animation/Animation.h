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

	Bone* FindBone(const std::string& name);

	inline float GetTicksPerSecond() { return this->ticksPerSecond; }
	inline float GetDuration() { return this->duration; }
	inline const AssimpNodeData& GetRootNode() { return this->rootNode; }
	inline const std::map<std::string, Mesh::BoneInfo>& GetBoneIDMap() { return this->boneInfoMap; }

private:
	float duration;
	int ticksPerSecond;
	std::vector<Bone> bones;
	AssimpNodeData rootNode;
	std::map<std::string, Mesh::BoneInfo> boneInfoMap;

	void ReadMissingBones(const aiAnimation* animation, Model& model);
	void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
};
