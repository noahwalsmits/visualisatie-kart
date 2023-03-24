#include "Shader.h"
#include<fstream>
#include<iostream>
#include <glm/gtc/type_ptr.hpp>

GLuint createShader(const std::string &shaderName, GLenum shaderType) {
	//get shader source code as text
	std::string shaderPath = "shaders/" + shaderName;
	std::ifstream shaderFile(shaderPath);
	std::string shaderData((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	const char* cShaderData = shaderData.c_str();

	//create shader
	GLuint shaderID = glCreateShader(shaderType); //create the shader
	glShaderSource(shaderID, 1, &cShaderData, NULL); //insert the source code read from the file
	glCompileShader(shaderID); //compile shader

	//check for compile errors
	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status); //check compilation
	if (status == GL_FALSE) { //print error if something went wrong
		int length, charsWritten;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length); //get error length
		char* infolog = new char[length + 1];
		memset(infolog, 0, length + 1);
		glGetShaderInfoLog(shaderID, length, &charsWritten, infolog); //get error message
		std::cout << "Error compiling shader:\n" << infolog << std::endl;
		delete[] infolog;
	}

	return shaderID;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	//create shader program
	this->ID = glCreateProgram();

	//create shader and attach it to this program ID
	GLint vertexShaderId = createShader(vertexPath, GL_VERTEX_SHADER);
	glAttachShader(this->ID, vertexShaderId);

	//create shader and attach it to this program ID
	GLint fragmentShaderId = createShader(fragmentPath, GL_FRAGMENT_SHADER);
	glAttachShader(this->ID, fragmentShaderId);

	//link the program
	glLinkProgram(this->ID); 

	//check for program compile errors
	GLint status;
	glGetProgramiv(this->ID, GL_COMPILE_STATUS, &status); //check compilation
	if (status == GL_FALSE) { //print error if something went wrong
		int length, charsWritten;
		glGetProgramiv(this->ID, GL_INFO_LOG_LENGTH, &length); //get error length
		char* infolog = new char[length + 1];
		memset(infolog, 0, length + 1);
		glGetProgramInfoLog(this->ID, length, &charsWritten, infolog); //get error message
		std::cout << "Error compiling shader:\n" << infolog << std::endl;
		delete[] infolog;
	}

	//the shaders are linked to the program so these can be deleted
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
}

void Shader::use() {
	glUseProgram(this->ID);
}

void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
