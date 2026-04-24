#pragma once
#include "BaseLight.h"


struct PointLightSetup
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 position;
    float linearDecay;
    float quadraticDecay;
    float constantDecay = 1.0f;
};

class PointLight :
    public BaseLight
{
public:
    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
        glm::vec3 position, float linearDecay, float quadraticDecay, float constantDecay = 1.0f);
    PointLight(PointLightSetup setup);
    void bindUniforms(GLuint shaderProgram, int index) override;
protected:
    glm::vec3 position;
    float constantDecay;
    float linearDecay;
    float quadraticDecay;

    void bindDecayUniforms(GLuint shaderProgram, std::string baseName);
};

