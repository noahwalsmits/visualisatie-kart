#include "Animation.h"
#include <iostream>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Animation::Animation(const std::string& animationPath, Model* model)
{
	std::cout << "\tLoading animation: " << animationPath << std::endl;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	assert(scene->mNumAnimations > 0);
	auto animation = scene->mAnimations[0];
	this->duration = animation->mDuration;
	this->ticksPerSecond = animation->mTicksPerSecond;
	ReadHeirarchyData(this->rootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
}

Bone* Animation::FindBone(const std::string& name)
{
	auto iterator = std::find_if(this->bones.begin(), this->bones.end(),
		[&](const Bone& Bone)
		{
			return Bone.GetBoneName() == name;
		}
	);
	if (iterator == this->bones.end())
	{
		return nullptr;
	}
	else
	{
		return &(*iterator);
	}
}

void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
{
	int size = animation->mNumChannels;

	auto& modelBoneInfoMap = model.getBoneInfoMap();
	int& boneCount = model.getBoneCount();

	//reading channels(bones engaged in an animation and their keyframes)
	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (modelBoneInfoMap.find(boneName) == modelBoneInfoMap.end())
		{
			modelBoneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		this->bones.push_back(Bone(channel->mNodeName.data, modelBoneInfoMap[channel->mNodeName.data].id, channel));
	}

	this->boneInfoMap = modelBoneInfoMap;
}

void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
{
	assert(src);

	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHeirarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}
