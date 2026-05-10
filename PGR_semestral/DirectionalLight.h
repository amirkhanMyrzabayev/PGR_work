#pragma once
#include "BaseLight.h"

/// @brief Helper struct to easily pass all starting settings for a directional light.
struct DirLightSetup
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
};

/// @brief A light that shines in one specific direction from far away.
class DirectionalLight :
    public BaseLight
{
public:
    /// @brief Creates a directional light using separate color and direction values.
    /// @param ambient Soft background color.
    /// @param diffuse Main light color.
    /// @param specular Color of shiny reflections.
    /// @param _direction The 3D direction the light is pointing towards.
    DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 _direction);

    /// @brief Creates a directional light using a filled setup struct.
    /// @param setup Struct containing all the needed colors and direction.
    DirectionalLight(DirLightSetup setup);

    /// @brief Sends the light's direction and current colors to the shader.
    /// @param shaderProgram The OpenGL ID of the shader.
    /// @param index The array index for this light in the shader (dirLights[index]).
    void bindUniforms(GLuint shaderProgram, int index) override;

    /// @brief Rotates the light direction to simulate a day/night cycle
    /// @param time Total time passed, used to calculate the new rotation angle.
    void update(float time) override;

    float getSpeed() { return slowSpeed; };

private:
    glm::vec3 direction;
    float slowSpeed = 0.1f; ///< Speed multiplier for how fast the day/night cycle rotates.
};