/**

    @file      AnimatedObject.h
    @brief     Class for Animated Object that can move.  
    @details   ~
    @author    Amirkhan Myrzabayev
    @date      10.05.2026
    @copyright © Amirkhan Myrzabayev, 2026. All right reserved.

**/
#pragma once


#pragma once

#pragma once
#include "SpriteObject.h"

/// @brief An object that automatically moves in an oval path (orbit) and can drag a 2D sprite behind it.
class AnimatedObject :
    public Object
{
public:
    /// @brief Creates the moving object and sets up its orbit path.
    /// @param setup Struct containing start position, orbit sizes, and movement speed.
    /// @param shaderManager Reference to get the shader program.
    /// @param meshManager Reference to get the 3D model.
    /// @param animation Optional 2D sprite (like a trail or fire) that follows behind this object.
    AnimatedObject(const AnimatedObjectSetup& setup, ShaderManager& shaderManager, MeshManager& meshManager,
        SpriteObject* animation = nullptr);

    /// @brief Calculates the new position on the oval orbit and rotates the object to face forward.
    /// @param time Time passed since the last frame (deltaTime).
    void update(float time) override;

    /// @brief Draws the 3D object and its attached trailing sprite if animation is running.
    /// @param view The camera's view matrix.
    /// @param proj The camera's projection matrix.
    /// @param viewPos The 3D position of the camera.
    void draw(const glm::mat4& view, const glm::mat4& proj,
        const glm::vec3& viewPos) override;

    /// @brief Starts or stops the object from moving along its path.
    void switchAnimation() { isAnimating = !isAnimating; };

    float getYaw() { return yaw; };

private:
    SpriteObject* animatedSprtie; ///< The attached 2D image following the object.
    glm::vec3 centerOrbit;        ///< The central 3D point that the object flies around.
    glm::vec3 startOrientation;
    float animationOffset = 0.0f; ///< How far behind the object the attached sprite is placed.
    float radiusX;                ///< Width of the oval orbit along the X-axis.
    float radiusZ;                ///< Length of the oval orbit along the Z-axis.
    float speed;                  ///< How fast the object travels around its path.
    float yaw;
    float elapsedTime = 0.0f;
    bool isAnimating = false;
};