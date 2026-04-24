#include "DirectionalLight.h"
#include <string>

DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 _direction) 
	: BaseLight(ambient, diffuse, specular), direction(_direction) {

}

DirectionalLight::DirectionalLight(DirLightSetup setup)
	: BaseLight(setup.ambient, setup.diffuse, setup.specular), direction(setup.direction)
{}

void DirectionalLight::bindUniforms(GLuint shaderProgram, int index) {
	std::string dirLightName = "dirLights[" + std::to_string(index) + "]";
	
	bindColorUniforms(shaderProgram, dirLightName);

	std::string directionName = dirLightName + ".direction";
	GLint dirLightLoc = glGetUniformLocation(shaderProgram, directionName.c_str());
	glUniform3fv(dirLightLoc, 1, glm::value_ptr(direction));
}