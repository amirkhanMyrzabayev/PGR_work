#pragma once
#include <vector>
#include <string>
#include "pgr.h"

/// @brief Draws a background sky (cubemap) around the scene using a full-screen 2D quad.
class Skybox
{
public:
	/// @brief Loads the 6 images to make a cubemap and sets up the flat rectangle for drawing.
	/// @param faces List of exactly 6 file paths for the cube sides (right, left, top, bottom, front, back).
	/// @param ShaderProgram The OpenGL ID of the shader program used to draw the sky.
	Skybox(const std::vector<std::string>& faces, GLuint ShaderProgram);

	~Skybox();

	/// @brief Draws the skybox so it looks like it is infinitely far away.
	/// @param view The camera's view matrix (we only use the rotation part so you can't walk closer to the sky).
	/// @param projection The camera's projection matrix.
	void draw(const glm::mat4& view, const glm::mat4& projection);

private:
	GLuint vao;           ///< Vertex Array Object for the 2D screen shape.
	GLuint vbo;           ///< Vertex Buffer Object storing the 4 corner points.
	GLuint textureID;     ///< OpenGL ID for the loaded 3D cubemap texture.
	GLuint shaderProgram; ///< OpenGL ID of the shader program.
};