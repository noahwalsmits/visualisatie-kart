#pragma once
#include <string>

class Shader {
public:
	unsigned int ID;

	Shader(const std::string &vertexPath, const std::string &fragmentPath);
	void use();
	void setBool(const std::string &name, bool value);
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);
};