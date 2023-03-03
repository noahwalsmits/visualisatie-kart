#include "RenderModel.h"
#include <fstream>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>>

RenderModel::RenderModel(std::string fileName) {
	fileName = "assets/" + fileName;
	std::ifstream modelFile(fileName.c_str());
	if (!modelFile.is_open()) {
		std::cout << "failed to load " << fileName;
		return;
	}
	
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

	std::vector<Vertex> finalVertices;
	std::vector< glm::vec3 > out_vertices;
	std::vector <glm::vec2> out_uvs;

	/***READ FILE***/
	while (true) {
		std::string line;
		std::getline(modelFile, line);
		if (modelFile.eof()) {
			break;
		}
		//std::cout << line << std::endl;//
		
		//read the header of each line
		char temp_header[3]; //dont care about anything more than 3 (including end char) characters
		sscanf_s(line.c_str(), "%s", temp_header, sizeof(temp_header));
		std::string header(temp_header);
		//std::cout << "header=|" << header << "|" << std::endl;//

		line = line.substr(header.length()); //remove header from line
		
		if (header == "v") { //read vertice
			glm::vec3 vertex;
			sscanf_s(line.c_str(), "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
			//std::cout << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;//
		}
		else if (header == "vt") { //read uv
			glm::vec2 uv;
			sscanf_s(line.c_str(), "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
			//std::cout << uv.x << " " << uv.y << std::endl;//
		}
		else if (header == "vn") { //read normal
			glm::vec3 normal;
			sscanf_s(line.c_str(), "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_vertices.push_back(normal);
			std::cout << normal.x << " " << normal.y << " " << normal.z << std::endl;//
		}
		else if (header == "f") { //read face
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3];
			//egg has no normals
			int matches = sscanf_s(line.c_str(), "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
			if (matches != 6) {
				std::cout << "error reading face" << std::endl;
				return;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
		}
	}

	/***INDEXING***/
	for (unsigned int i = 0; i < vertexIndices.size(); i++) { //for each vertex of each face
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}


	for (int i = 0; i < out_vertices.size(); i++) {
		finalVertices.push_back(Vertex(out_vertices[i], glm::vec4(1, 0, 0, 1), out_uvs[i]));
	}
	this->vertices = finalVertices;
}

void RenderModel::draw() {
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), &this->vertices[0].position);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), &this->vertices[0].color);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), &this->vertices[0].textureCoords);
	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	//glDrawElements(GL_TRIANGLES, this->vertices.size(), GL_UNSIGNED_INT, 0);//
}