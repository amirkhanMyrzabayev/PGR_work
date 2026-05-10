#pragma once
#include "BaseLight.h"

/// @brief Helper struct to easily pass all starting settings for a point light.
struct PointLightSetup
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 position;
    float linearDecay;    ///< Fades the light out steadily as it gets further away.
    float quadraticDecay; ///< Fades the light out much faster over longer distances.
    float constantDecay = 1.0f;
};

/// @brief A light that shines from a specific point in all directions.
class PointLight :
    public BaseLight
{
public:
    /// @brief Creates a point light using separate color, position, and fading values.
    /// @param ambient Soft background color.
    /// @param diffuse Main light color.
    /// @param specular Color of shiny reflections.
    /// @param position Where the light is located in the 3D world.
    /// @param linearDecay How steadily the light fades over distance.
    /// @param quadraticDecay How sharply the light fades out over longer distances.
    /// @param constantDecay Base strength of the light (usually left at 1.0).
    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
        glm::vec3 position, float linearDecay, float quadraticDecay, float constantDecay = 1.0f);

    /// @brief Creates a point light using a pre-filled setup struct.
    /// @param setup Struct containing all the needed settings.
    PointLight(PointLightSetup setup);

    /// @brief Sends the light's colors, position, and fading rules to the shader.
    /// @param shaderProgram The OpenGL ID of the shader.
    /// @param index The array index for this light in the shader (pointLights[index]).
    void bindUniforms(GLuint shaderProgram, int index) override;

protected:
    glm::vec3 position;
    float constantDecay;
    float linearDecay;
    float quadraticDecay;

    /// @brief Helper function to send just the position and fading rules to the shader.
    /// @param shaderProgram The OpenGL ID of the shader.
    /// @param baseName The name of the struct in the shader code (like "pointLights[0]").
    void bindDecayUniforms(GLuint shaderProgram, std::string baseName);
};