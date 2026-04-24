#include "PointLight.h"

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
    glm::vec3 _position, float _linearDecay, float _quadraticDecay, float _constantDecay) 
    : BaseLight(ambient, diffuse, specular), 
    position(_position), linearDecay(_linearDecay), quadraticDecay(_quadraticDecay), constantDecay(_constantDecay)
{}

PointLight::PointLight(PointLightSetup setup)
    : BaseLight(setup.ambient, setup.diffuse, setup.specular), position(setup.position),
    linearDecay(setup.linearDecay), quadraticDecay(setup.quadraticDecay), constantDecay(setup.constantDecay)
{}

void PointLight::bindDecayUniforms(GLuint shaderProgram, std::string baseName) {
    std::string tempName = baseName + ".position";
    GLint pointLightLoc = glGetUniformLocation(shaderProgram, tempName.c_str());
    glUniform3fv(pointLightLoc, 1, glm::value_ptr(position));

    tempName = baseName + ".constant";
    pointLightLoc = glGetUniformLocation(shaderProgram, tempName.c_str());
    glUniform1f(pointLightLoc, constantDecay);

    tempName = baseName + ".linear";
    pointLightLoc = glGetUniformLocation(shaderProgram, tempName.c_str());
    glUniform1f(pointLightLoc, linearDecay);

    tempName = baseName + ".quadratic";
    pointLightLoc = glGetUniformLocation(shaderProgram, tempName.c_str());
    glUniform1f(pointLightLoc, quadraticDecay);
}

void PointLight::bindUniforms(GLuint shaderProgram, int index) {
    std::string pointLightName = "pointLights[" + std::to_string(index) + "]";
 
    bindColorUniforms(shaderProgram, pointLightName);
    bindDecayUniforms(shaderProgram, pointLightName);
}
