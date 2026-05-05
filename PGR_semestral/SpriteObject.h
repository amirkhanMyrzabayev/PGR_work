#pragma once
#include "Object.h"

struct FrameLocations {
    GLint columnsLoc;
    GLint rowsLoc;
    GLint currentFrameLoc;
};


class SpriteObject :
    public Object
{
public:
    SpriteObject(const SpriteObjectSetup& setup, ShaderManager& shaderManager, MeshManager& meshManager);
    void update(float deltaTime);
    void draw(const glm::mat4 view, const glm::mat4& proj, const glm::vec3& viewPos) override;
    glm::vec3 getStartOrientation() { return startOrientation; };

private:
    size_t columns;
    size_t rows;
    float fps;
    size_t currentFrame = 0;
    float frameTimer = 0.0f;
    glm::vec3 startOrientation;
    FrameLocations frameLocations;
};

