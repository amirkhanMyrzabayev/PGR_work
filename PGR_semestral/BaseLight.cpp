#include "BaseLight.h"
#include <iostream>

BaseLight::BaseLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
	ambientColor(ambient), diffuseColor(diffuse), specularColor(specular),
	baseAmbient(ambient), baseDiffuse(diffuse), baseSpecular(specular)
{}

BaseLight::~BaseLight() {};

void BaseLight::update(float time) {}

void BaseLight::bindColorUniforms(GLuint shaderProgram, std::string baseName) {
	GLint ambientLocation;
	GLint diffuseLocation;
	GLint specularLocation;


	std::string ambientName = baseName + ".ambient";
	ambientLocation = glGetUniformLocation(shaderProgram, ambientName.c_str());
	std::string diffuseName = baseName + ".diffuse";
	diffuseLocation = glGetUniformLocation(shaderProgram, diffuseName.c_str());
	std::string specularName = baseName + ".specular";
	specularLocation = glGetUniformLocation(shaderProgram, specularName.c_str());
	
	glUniform3fv(ambientLocation, 1, glm::value_ptr(ambientColor));
	glUniform3fv(diffuseLocation, 1, glm::value_ptr(diffuseColor));
	glUniform3fv(specularLocation, 1, glm::value_ptr(specularColor));	
	GLint activeLocation = glGetUniformLocation(shaderProgram, (baseName + ".isActive").c_str());
	glUniform1i(activeLocation, isActive ? 1 : 0);


}