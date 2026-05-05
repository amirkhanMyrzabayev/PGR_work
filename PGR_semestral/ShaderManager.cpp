#include "ShaderManager.h"


ShaderManager::~ShaderManager() {
	for (auto& shader : programs) {
		glDeleteProgram(shader.second);
	}
	programs.clear();
}

GLuint ShaderManager::getShaderProgram(const std::string& shaderName) {
	if (programs.find(shaderName) != programs.end()) {
		return programs[shaderName];
	}
	std::string vertPath = shaderName + ".vert";
	std::string fragPath = shaderName + ".frag";
	GLuint shaders[] = {
		pgr::createShaderFromFile(GL_VERTEX_SHADER, vertPath),
		pgr::createShaderFromFile(GL_FRAGMENT_SHADER, fragPath),
		0
	};
	GLuint shaderProgram = pgr::createProgram(shaders);
	programs[shaderName] = shaderProgram;
	return shaderProgram;
}

void ShaderManager::setFogInShaders(glm::vec3 fogColor, float fogStart, float fogEnd) {
	for (auto& shader : programs) {
		glUseProgram(shader.second);
		GLuint fogColorPos = glGetUniformLocation(shader.second, "fogColor");
		if (fogColorPos == -1) {
			glUseProgram(0);
			continue;
		}
		GLuint fogStartPos = glGetUniformLocation(shader.second, "fogStart");
		GLuint fogEndPos = glGetUniformLocation(shader.second, "fogEnd");
		glUniform3fv(fogColorPos, 1, glm::value_ptr(fogColor));
		glUniform1f(fogStartPos, fogStart);
		glUniform1f(fogEndPos, fogEnd);
		glUseProgram(0);
	}
}