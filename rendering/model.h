#pragma once
#include <vector>
#include <string>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Shader.h"

#include <glad/glad.h>
#include <map>

class Model 
{
public:
	glm::vec3 position; //location to draw the model
	float rotationYaw = 0.0f; //z-axis rotation in degrees
	float rotationPitch = 0.0f; //y-axis rotation in degrees
	float rotationRoll = 0.0f; //x-axis rotation in degrees
	glm::vec3 offset; //offset to position applied after the rotation
	float scale = 1.0f; //size multplier of the model

	/*Loads a 3D model using ASSIMP*/
	Model(std::string const& path, glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f));
	virtual void draw(Shader &shader);

	auto& getBoneInfoMap() { return this->boneInfoMap; }
	int& getBoneCount() { return this->boneCount; }

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Mesh::Texture> texturesLoaded;
	std::map<std::string, Mesh::BoneInfo> boneInfoMap;
	int boneCount = 0;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(const char* path, const std::string& directory);
	void ExtractBoneWeightForVertices(std::vector<Mesh::Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
	void SetVertexBoneData(Mesh::Vertex& vertex, int boneID, float weight);
};