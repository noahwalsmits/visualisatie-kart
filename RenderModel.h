#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>

class RenderModel
{
public:
	class Vertex {
	public:
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoords;
		Vertex(const glm::vec3& position, const glm::vec4& color, const glm::vec2& textureCoords) : position(position), color(color), textureCoords(textureCoords) {}
	};

	RenderModel(std::string fileName);
	void draw();
private:
	std::vector<Vertex> vertices;
};

