#include "Object.h"
#include "ObjLoader.h"

Object::Object(const std::string& filePath, GLuint _shaderProgram, MeshManager& meshManager) : shaderProgram(_shaderProgram){
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	this->mesh = meshManager.getMesh(filePath, _shaderProgram);
}

Object::Object(const std::string& filePath, GLuint _shaderProgram, 
				MeshManager& meshManager,
				const glm::vec3& newPos, 
				const glm::vec3& newRotation, 
				const glm::vec3& newScale) : Object(filePath, _shaderProgram, meshManager){
	setSRP(newPos, newRotation, newScale);
}


void Object::setPosition(const glm::vec3& newPos) { position = newPos; }
void Object::setRotation(const glm::vec3& newRot) { rotation = newRot; }
void Object::setScale(const glm::vec3& newScale) { scale = newScale; }

void Object::setSRP(const glm::vec3& newPos, const glm::vec3& newRotation, const glm::vec3& newScale) {
	setPosition(newPos);
	setRotation(newRotation);
	setScale(newScale);
}

void Object::draw() {
	glm::mat4 modelR = glm::mat4(1.0f);
	modelR = glm::translate(modelR, position);
	modelR = glm::rotate(modelR, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelR = glm::rotate(modelR, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelR = glm::rotate(modelR, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelR = glm::scale(modelR, scale);

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelR)));
	GLint modelLocation = glGetUniformLocation(shaderProgram, "model");
	GLint normalMatrixLocation = glGetUniformLocation(shaderProgram, "normalMatrix");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelR));
	glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->numVertices);
	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

