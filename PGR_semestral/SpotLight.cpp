#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                    glm::vec3 _direction, glm::vec3 _position,
                    float innerCutOffAngle, float outerCutOffAngle,
                    float _linearDecay, float _quadraticDecay, float _constantDecay) 
    : PointLight(ambient, diffuse, specular, _position, _linearDecay, _quadraticDecay, _constantDecay),
    direction(_direction), innerCutOffCos(glm::cos(innerCutOffAngle)), outerCutOffCos(glm::cos(outerCutOffAngle))
{ }

SpotLight::SpotLight(SpotLightSetup setup) 
    : PointLight(setup.ambient, setup.diffuse, setup.specular, setup.position, 
                setup.linearDecay, setup.quadraticDecay, setup.constantDecay),
    direction(setup.direction), innerCutOffCos(glm::cos(setup.innerCutOffAngle)), outerCutOffCos(glm::cos(setup.outerCutOffAngle))
{}

void SpotLight::bindUniforms(GLuint shaderProgram, int index) {
    std::string spotName = "spotLights[" + std::to_string(index) + "]";
    bindColorUniforms(shaderProgram, spotName);
    bindDecayUniforms(shaderProgram, spotName);


    std::string tempName = spotName + ".direction";
    GLint spotLightLoc = glGetUniformLocation(shaderProgram, tempName.c_str());
    glUniform3fv(spotLightLoc, 1, glm::value_ptr(direction));

    tempName = spotName + ".innerCutOff";
    spotLightLoc = glGetUniformLocation(shaderProgram, tempName.c_str());
    glUniform1f(spotLightLoc, innerCutOffCos);

    tempName = spotName + ".outerCutOff";
    spotLightLoc = glGetUniformLocation(shaderProgram, tempName.c_str());
    glUniform1f(spotLightLoc, outerCutOffCos);
}