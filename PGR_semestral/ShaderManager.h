#pragma once
#include <unordered_map>
#include <string>
#include "pgr.h"

class ShaderManager
{
public:
	~ShaderManager();
	GLuint getShaderProgram(const std::string& shaderName);

private:
	std::unordered_map<std::string, GLuint> programs;
};

