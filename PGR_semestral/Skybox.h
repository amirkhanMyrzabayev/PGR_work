#pragma once
#include <vector>
#include <string>
#include "pgr.h"

class Skybox
{
public:
	Skybox(const std::vector<std::string>& faces, GLuint ShaderProgram);
	~Skybox();
	void draw(const glm::mat4& view, const glm::mat4& projection);
private:
	GLuint vao, vbo, textureID, shaderProgram;
	
};

