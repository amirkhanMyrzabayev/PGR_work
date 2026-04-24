#pragma once
#include <string>
#include "pgr.h"


class BaseLight
{
public:
	BaseLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
	virtual ~BaseLight();
	virtual void update(float time);
	virtual void bindUniforms(GLuint shaderProgram, int index) = 0;

protected:
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	bool isActive = true;
	void bindColorUniforms(GLuint shaderProgram, std::string baseName);
};

