#pragma once
#include "Object.h"

class AnimatedObject :
    public Object
{
public:
    AnimatedObject(const AnimatedObjectSetup& setup, ShaderManager& shaderManager, MeshManager& meshManager);
    void update(float time);
private:
    glm::vec3 centerOrbit;
    glm::vec3 startOrientation;
    float radiusX;
    float radiusZ;
    float speed;
    float elapsedTime = 0.0f;
    bool isAnimating = true;
};

