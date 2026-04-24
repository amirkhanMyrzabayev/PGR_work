#include "BaseLight.h"

BaseLight::BaseLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
	ambientColor(ambient), diffuseColor(diffuse), specularColor(specular) {}

BaseLight::~BaseLight() {};

void BaseLight::update(float time) {}

void BaseLight::bindColorUniforms(GLuint shaderProgram, std::string baseName) {
	GLint lightLocation;

	std::string ambientName = baseName + ".ambient";
	lightLocation = glGetUniformLocation(shaderProgram, ambientName.c_str());
	glUniform3fv(lightLocation, 1, glm::value_ptr(ambientColor));

	std::string diffuseName = baseName + ".diffuse";
	lightLocation = glGetUniformLocation(shaderProgram, diffuseName.c_str());
	glUniform3fv(lightLocation, 1, glm::value_ptr(diffuseColor));

	std::string specularName = baseName + ".specular";
	lightLocation = glGetUniformLocation(shaderProgram, specularName.c_str());
	glUniform3fv(lightLocation, 1, glm::value_ptr(specularColor));
}