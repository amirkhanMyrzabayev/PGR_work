#pragma once
#include "SpriteObject.h"


class AnimatedObject :
    public Object
{
public:
    AnimatedObject(const AnimatedObjectSetup& setup, ShaderManager& shaderManager, MeshManager& meshManager, 
                    SpriteObject* animation = nullptr);
    void update(float time) override;
    void draw(const glm::mat4& view, const glm::mat4& proj,
                const glm::vec3& viewPos) override;
    void switchAnimation() { isAnimating = !isAnimating; };

    float getYaw() { return yaw; };
private:
    SpriteObject* animatedSprtie;
    glm::vec3 centerOrbit;
    glm::vec3 startOrientation;
    float animationOffset = 0.0f;
    float radiusX;
    float radiusZ;
    float speed;
    float yaw;
    float elapsedTime = 0.0f;
    bool isAnimating = false;
};

