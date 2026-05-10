/**

    @file      SpotLight.h
    @brief     Child class of PointLight that adds cut off angles.
    @details   ~
    @author    Amirkhan Myrzabayev
    @date      10.05.2026
    @copyright © Amirkhan Myrzabayev, 2026. All right reserved.

**/
#pragma once

#include "PointLight.h"

/// @brief Helper struct to easily pass all starting settings for a spotlight.
struct SpotLightSetup
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    glm::vec3 direction;
    glm::vec3 position;

    float innerCutOffAngle; ///< Angle (in radians) where the light is at 100% full strength.
    float outerCutOffAngle; ///< Angle (in radians) where the light completely fades out, creating a soft edge.

    float linearDecay;
    float quadraticDecay;
    float constantDecay = 1.0f;
};

/// @brief A light that shines in a cone shape (like a flashlight or a car headlight).
class SpotLight :
    public PointLight
{
public:
    /// @brief Creates a spotlight using separate color, position, direction, and cone angles.
    /// @param ambient Soft background color.
    /// @param diffuse Main light color.
    /// @param specular Color of shiny reflections.
    /// @param _direction Which way the spotlight is pointing in the 3D world.
    /// @param _position Where the spotlight is located.
    /// @param innerCutOffAngle The size of the bright inner cone.
    /// @param outerCutOffAngle The size of the fading outer cone.
    /// @param _linearDecay How steadily the light fades over distance.
    /// @param _quadraticDecay How sharply the light fades out over longer distances.
    /// @param _constantDecay Base strength of the light (usually left at 1.0).
    SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
        glm::vec3 _direction, glm::vec3 _position,
        float innerCutOffAngle, float outerCutOffAngle,
        float _linearDecay, float _quadraticDecay, float _constantDecay = 1.0f);

    /// @brief Creates a spotlight using a pre-filled setup struct.
    /// @param setup Struct containing all the needed settings.
    SpotLight(SpotLightSetup setup);

    /// @brief Sends the spotlight's colors, position, direction, and cone limits to the shader.
    /// @param shaderProgram The OpenGL ID of the shader.
    /// @param index The array index for this light in the shader (spotLights[index]).
    void bindUniforms(GLuint shaderProgram, int index) override;

private:
    glm::vec3 direction;
    float innerCutOffCos; ///< Stored as a cosine value 
    float outerCutOffCos; ///< Stored as a cosine value
};