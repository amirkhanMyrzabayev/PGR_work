/**

    @file      SpriteObject.h
    @brief     Child class of Object that is used for drawing animated sprites.
    @details   ~
    @author    Amirkhan Myrzabayev
    @date      10.05.2026
    @copyright © Amirkhan Myrzabayev, 2026. All right reserved.

**/
#pragma once

#include "Object.h"

/// @brief Stores OpenGL memory locations for sprite animation variables.
struct FrameLocations {
    GLint columnsLoc;      ///< Shader location for the number of columns in the sprite sheet.
    GLint rowsLoc;         ///< Shader location for the number of rows in the sprite sheet.
    GLint currentFrameLoc; ///< Shader location for the currently active animation frame.
};

/// @brief A 2D flat image that can play animations from a sprite sheet, or act as a HUD element.
class SpriteObject :
    public Object
{
public:
    /// @brief Creates the animated sprite and prepares its shader variables.
    /// @param setup Struct containing settings like grid size (columns/rows) and animation speed (fps).
    /// @param shaderManager Reference to get the shader program.
    /// @param meshManager Reference to get the 2D flat mesh.
    SpriteObject(const SpriteObjectSetup& setup, ShaderManager& shaderManager, MeshManager& meshManager);

    /// @brief Advances the animation to the next frame based on the FPS.
    /// @param deltaTime Time passed since the last frame.
    void update(float deltaTime) override;
    
    /// @brief Draws the animated 2D sprite inside the 3D world.
    /// @param view The camera's view matrix.
    /// @param proj The camera's projection matrix.
    /// @param viewPos The 3D position of the camera.
    void draw(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos) override;

    /// @brief Draws the sprite as a HUD clock directly on the screen, ignoring 3D depth.
    /// @param view The camera's view matrix.
    /// @param elapsedTime Total time passed, used to rotate the clock hands in the shader.
    /// @param sunSpeed The speed of the day/night cycle so the clock matches the sun.
    void drawClock(const glm::mat4& view, float elapsedTime, float sunSpeed);

    bool isClock() { return timeLoc != -1; };
    glm::vec3 getStartOrientation() { return startOrientation; };

private:
    size_t columns;           ///< How many columns of images are in the sprite sheet.
    size_t rows;              ///< How many rows of images are in the sprite sheet.
    float fps;                ///< Frames per second (how fast the animation plays).
    size_t currentFrame = 0;  ///< The index of the picture currently being shown.
    float frameTimer = 0.0f;  ///< Keeps track of time to know exactly when to switch frames.
    glm::vec3 startOrientation;
    FrameLocations frameLocations;
    GLint timeLoc = -1;       ///< Shader location for elapsed time (used by the clock).
    GLint speedLoc;           ///< Shader location for the day/night speed (used by the clock).
};