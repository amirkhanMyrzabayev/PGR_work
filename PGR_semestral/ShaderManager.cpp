#include "ShaderManager.h"


ShaderManager::~ShaderManager() {
	for (auto shader : programs) {
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