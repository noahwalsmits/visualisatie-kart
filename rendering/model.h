#pragma once
#include <vector>
#include <string>
#include <assimp/scene.h>

#include "Mesh.h"
#include "Shader.h"

#include <glad/glad.h>
#include <map>

/*Loads a 3D model using ASSIMP*/
class Model 
{
public:
	std::vector<Mesh::Texture> texturesLoaded;
	Model(std::string const &path, glm::vec3 position)
	{
		this->position = position;
		this->loadModel(path);
	}
	void draw(Shader &shader, GLuint& modelUniform);
	auto& getBoneInfoMap() { return this->boneInfoMap; }
	int& getBoneCount() { return this->boneCounter; }

private:
	glm::vec3 position;
	std::vector<Mesh> meshes;
	std::string directory;

	std::map<std::string, Mesh::BoneInfo> boneInfoMap;
	int boneCounter = 0;
	void SetVertexBoneDataToDefault(Mesh::Vertex& vertex);
	void SetVertexBoneData(Mesh::Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<Mesh::Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(const char* path, const std::string& directory);
};