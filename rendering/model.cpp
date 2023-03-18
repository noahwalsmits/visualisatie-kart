#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Model::draw(Shader &shader, GLuint& modelUniform)
{
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), this->position); //TODO store position and let it be changed at runtime
	//modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f)); //TODO add options for rotation and scale
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	//draw meshes
	for (unsigned int i = 0; i < this->meshes.size(); i++) 
	{
		this->meshes[i].draw(shader);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	//aiProcess_Triangulate transforms all primitive shapes into triangles, if they are not already
	//aiProcess_FlipUVs flips textures on the y-axis
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	//const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP error importing model: " << importer.GetErrorString() << std::endl;
		return;
	}

	this->directory = path.substr(0, path.find_last_of('/')); //save the model directory

	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//process all of the node's meshes if it has any
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(processMesh(mesh, scene));
	}

	//process all of the node's childrens meshes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Mesh::Texture> textures;

	//create vertexes
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		//process positions
		glm::vec3 tempPositions;
		tempPositions.x = mesh->mVertices[i].x;
		tempPositions.y = mesh->mVertices[i].y;
		tempPositions.z = mesh->mVertices[i].z;

		//process normals
		glm::vec3 tempNormals;
		if (mesh->HasNormals())
		{
			tempNormals.x = mesh->mNormals[i].x;
			tempNormals.y = mesh->mNormals[i].y;
			tempNormals.z = mesh->mNormals[i].z;
		}

		//process texture coordinates
		glm::vec2 tempTexCoords;
		if (mesh->mTextureCoords[0]) //check if it has texture coordinates
		{
			tempTexCoords.x = mesh->mTextureCoords[0][i].x;
			tempTexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else 
		{
			tempTexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(Mesh::Vertex(tempPositions, tempNormals, tempTexCoords));
	}

	//process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Mesh::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Mesh::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Mesh::Texture> textures;

	//iterate over each texture location of the given type and load it
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str); //get texture name
		bool skip = false; //skip if already loaded before
		for (unsigned int j = 0; j < this->texturesLoaded.size(); j++) //check if already loaded
		{
			if (std::strcmp(this->texturesLoaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(this->texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Mesh::Texture texture;
			texture.id = this->textureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			this->texturesLoaded.push_back(texture);
		}
	}
	return textures;
}

unsigned int Model::textureFromFile(const char* path, const std::string& directory)
{
	std::cout << "Loading texture: " << path << std::endl;//
	std::string fileName = std::string(path);
	fileName = directory + '/' + fileName;
	
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1) 
		{
			format = GL_RED;
		}
		else if (nrComponents == 3) 
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}
		
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}