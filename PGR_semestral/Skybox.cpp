#include "Skybox.h"

Skybox::Skybox(const std::vector<std::string>& faces, GLuint ShaderProgram) {
	const float screenCoords[] = {
	-1.0f, -1.0f, // Bottom-left
	 1.0f, -1.0f, // Bottom-right
	-1.0f,  1.0f, // Top-left
	 1.0f,  1.0f  // Top-right
	};
	shaderProgram = ShaderProgram;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), &screenCoords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	for (size_t i = 0; i < 6; i++) {
		if (!pgr::loadTexImage2D(faces[i], targets[i])) {
			return;
		}
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


void Skybox::draw(const glm::mat4& view, const glm::mat4& projection) {
	glDepthFunc(GL_LEQUAL);
	glUseProgram(shaderProgram);
	glm::mat4 viewRotOnly = glm::inverse(glm::mat4(glm::mat3(view)));
	GLuint viewLocation = glGetUniformLocation(shaderProgram, "view");
	GLuint projLocation = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewRotOnly));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(glm::inverse(projection)));
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDepthFunc(GL_LESS);
}