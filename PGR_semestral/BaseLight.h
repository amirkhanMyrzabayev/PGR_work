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
	void switchLight() { isActive = !isActive; };

protected:
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 baseAmbient;
	glm::vec3 baseDiffuse;
	glm::vec3 baseSpecular;
	bool isActive = true;
	void bindColorUniforms(GLuint shaderProgram, std::string baseName);
};

