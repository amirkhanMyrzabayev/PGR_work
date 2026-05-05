#include "SpriteObject.h"
#include <iostream>

SpriteObject::SpriteObject(const SpriteObjectSetup& setup, ShaderManager& shaderManager, MeshManager& meshManager)
	: Object(setup, shaderManager, meshManager), columns(setup.columns), rows(setup.rows), fps(setup.fps), startOrientation(setup.rotation)
{
	GLuint shaderProgram = getShaderProgram();
	frameLocations.columnsLoc = glGetUniformLocation(shaderProgram, "columns");
	frameLocations.rowsLoc = glGetUniformLocation(shaderProgram, "rows");
	frameLocations.currentFrameLoc = glGetUniformLocation(shaderProgram, "currentFrame");

}

void SpriteObject::update(float deltaTime) {
	frameTimer += deltaTime;
	float timePerFrame = 1.0f / fps;
	while (frameTimer >= timePerFrame) {
		frameTimer -= timePerFrame;
		currentFrame++;
		if (currentFrame >= (columns * rows)) {
			currentFrame = 0;
		}
	}
}

void SpriteObject::draw(const glm::mat4 view, const glm::mat4& proj,
	const glm::vec3& viewPos) {
	GLuint shaderProgram = getShaderProgram();
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(locations.viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(locations.projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	glUniform3fv(locations.viewPosLoc, 1, glm::value_ptr(viewPos));
	glUniform1i(frameLocations.columnsLoc, columns);
	glUniform1i(frameLocations.rowsLoc, rows);
	glUniform1i(frameLocations.currentFrameLoc, currentFrame);
	glm::mat4 modelR = glm::mat4(1.0f);
	modelR = glm::translate(modelR, position);
	modelR = glm::rotate(modelR, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelR = glm::rotate(modelR, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelR = glm::rotate(modelR, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelR = glm::scale(modelR, scale);
	glUniformMatrix4fv(locations.modelLoc, 1, GL_FALSE, glm::value_ptr(modelR));
	glBindVertexArray(mesh->vao);
	for (const auto& subMesh : mesh->subMeshes) {
		if (subMesh.material.diffuseTextureID != 0) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, subMesh.material.diffuseTextureID);
			
			glUniform1i(locations.diffuseMapLoc, 0);
		}
		glDrawArrays(GL_TRIANGLES, subMesh.startIndex, subMesh.numVertices);
	}
	glBindVertexArray(0);
}