#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "Shader.h"

class Mesh
{
	/***DATA CLASSES***/
public:
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;

		static const int MAX_BONE_INFLUENCE = 4;
		int boneIds[MAX_BONE_INFLUENCE]; //bone indexes that influence this vertex
		float boneWeights[MAX_BONE_INFLUENCE]; //weights from each bone

		Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2 texCoords) 
			: position(position), normal(normal), texCoords(texCoords) 
		{
			for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
			{
				boneIds[i] = -1;
				boneWeights[i] = 0.0f;
			}
		}
	};

	struct Texture {
	public:
		unsigned int id;
		std::string type;
		std::string path;
	};

	struct BoneInfo {
		int id; //id is index in finalBoneMatrices
		glm::mat4 offset; //offset matrix transforms vertex from model space to bone space
	};


	/***MESH***/
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader &shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

