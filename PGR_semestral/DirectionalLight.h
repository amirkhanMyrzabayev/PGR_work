#pragma once
#include "BaseLight.h"

struct DirLightSetup
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
};

class DirectionalLight :
    public BaseLight
{
public:
    DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 _direction);
    DirectionalLight(DirLightSetup setup);
    void bindUniforms(GLuint shaderProgram, int index) override;
private:
    glm::vec3 direction;
};

