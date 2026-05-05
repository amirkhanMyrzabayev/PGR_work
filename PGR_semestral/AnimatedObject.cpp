#include "AnimatedObject.h"

AnimatedObject::AnimatedObject(const AnimatedObjectSetup& setup, ShaderManager& shaderManager, MeshManager& meshManager) :
    Object(setup, shaderManager, meshManager),
    radiusX(setup.radiusX), radiusZ(setup.radiusZ), speed(setup.speed), startOrientation(getOrientation()),
    centerOrbit(glm::vec3(setup.position.x - setup.radiusX, setup.position.y, setup.position.z))
{}

void AnimatedObject::update(float deltaTime) {
    if (!isAnimating) return;
    elapsedTime += deltaTime;
    float angle = elapsedTime * speed;
    float newX = centerOrbit.x + (glm::cos(angle) * radiusX);
    float newZ = centerOrbit.z + (glm::sin(angle) * radiusZ);
    float newY = centerOrbit.y;
    setPosition(glm::vec3(newX, newY, newZ));

    float forwardX = -glm::sin(angle) * radiusX;
    float forwardZ = glm::cos(angle) * radiusZ;
    glm::vec2 forwardVec = glm::normalize(glm::vec2(forwardX, forwardZ));
    float yaw = glm::atan(forwardVec.x, forwardVec.y);
    setRotation(startOrientation+glm::vec3(0.0f, yaw, 0.0f));
}