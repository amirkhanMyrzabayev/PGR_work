/**

    @file      BaseLight.h
    @brief     Base class for all lights.
    @details   ~
    @author    Amirkhan Myrzabayev
    @date      10.05.2026
    @copyright © Amirkhan Myrzabayev, 2026. All right reserved.

**/
#pragma once

#include <string>
#include "pgr.h"

/// @brief Parent class for all types of lights. Cannot be created directly.
class BaseLight
{
public:
	/// @brief Sets up the starting colors for the light.
	/// @param ambient Soft background color of the light.
	/// @param diffuse Main color of the light that hits objects.
	/// @param specular Color of the shiny reflections caused by this light.
	BaseLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	virtual ~BaseLight();

	/// @brief Updates light logic every frame (like movement or blinking).
	/// @param time Total time passed since the app started.
	virtual void update(float time);

	/// @brief Sends the specific light data to the shader. Must be written by child classes.
	/// @param shaderProgram The OpenGL ID of the shader.
	/// @param index The array index of this light in the shader (e.g., 0 for pointLights[0]).
	virtual void bindUniforms(GLuint shaderProgram, int index) = 0;

	/// @brief Turns the light on if it is off, or off if it is on.
	void switchLight() { isActive = !isActive; };

protected:
	glm::vec3 ambientColor;  ///< Current ambient color (might change if light dims).
	glm::vec3 diffuseColor;  ///< Current diffuse color.
	glm::vec3 specularColor; ///< Current specular color.

	glm::vec3 baseAmbient;   ///< Original starting ambient color, saved in case we need to reset it.
	glm::vec3 baseDiffuse;   ///< Original starting diffuse color.
	glm::vec3 baseSpecular;  ///< Original starting specular color.

	bool isActive = true;    ///< True if the light is on.

	/// @brief Helper function to send the shared color and active state to the shader.
	/// @param shaderProgram The OpenGL ID of the shader.
	/// @param baseName The name of the struct in the GLSL shader code (like "dirLight").
	void bindColorUniforms(GLuint shaderProgram, std::string baseName);
};