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
		Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2 texCoords) : position(position), normal(normal), texCoords(texCoords) {}
	};

	struct Texture {
	public:
		unsigned int id;
		std::string type;
		std::string path;
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

