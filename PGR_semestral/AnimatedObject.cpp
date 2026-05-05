#include "AnimatedObject.h"

AnimatedObject::AnimatedObject(const AnimatedObjectSetup& setup, ShaderManager& shaderManager, MeshManager& meshManager,
                                std::unique_ptr<SpriteObject> animation) :
    Object(setup, shaderManager, meshManager),
    radiusX(setup.radiusX), radiusZ(setup.radiusZ), speed(setup.speed), startOrientation(getOrientation()),
    centerOrbit(glm::vec3(setup.position.x - setup.radiusX, setup.position.y, setup.position.z))
{
    if (animation != nullptr) {
        animatedSprtie = std::move(animation);
        animationOffset = setup.animationOffset;
    }
}

void AnimatedObject::draw(const glm::mat4 view, const glm::mat4& proj,
                            const glm::vec3& viewPos) {
    Object::draw(view, proj, viewPos);
    if (animatedSprtie && isAnimating) {
        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        animatedSprtie->draw(view, proj, viewPos);
        glDisable(GL_BLEND);
    }
}

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


    if (animatedSprtie) {
        //std::cout << "updated animation " << std::endl;
        glm::vec3 backward = glm::vec3(-forwardX, 0.0f, -forwardZ);
        animatedSprtie->setPosition(position + backward*animationOffset);
        animatedSprtie->setRotation(animatedSprtie->getStartOrientation() + glm::vec3(0.0f, yaw, 0.0f));
        animatedSprtie->update(deltaTime);
    }
}