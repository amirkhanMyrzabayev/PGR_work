#pragma once
#include "PointLight.h"

struct SpotLightSetup
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec3 direction;
    glm::vec3 position;

    float innerCutOffAngle;
    float outerCutOffAngle;

    float linearDecay;
    float quadraticDecay;
    float constantDecay = 1.0f;
};

class SpotLight :
    public PointLight
{
public:
    SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
            glm::vec3 _direction, glm::vec3 _position, 
            float innerCutOffAngle, float outerCutOffAngle,
            float _linearDecay, float _quadraticDecay, float _constantDecay = 1.0f);
    SpotLight(SpotLightSetup setup);
    void bindUniforms(GLuint shaderProgram, int index) override;

private:
    glm::vec3 direction;
    float innerCutOffCos;
    float outerCutOffCos;
};

