#include "Object.h"
#include "ObjLoader.h"

Object::Object(const std::string& filePath, GLuint _shaderProgram) : shaderProgram(_shaderProgram){
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uv_array;
	std::vector<glm::vec3> normals;
	ObjLoader::loadOBJ(filePath, vertices, uv_array, normals);
	numVertices = vertices.size();
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	GLint posLoc = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindVertexArray(0);
}

Object::Object(const std::string& filePath, GLuint _shaderProgram, 
				const glm::vec3& newPos, 
				const glm::vec3& newRotation, 
				const glm::vec3& newScale) : Object(filePath, _shaderProgram){
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
	GLint modelLocation = glGetUniformLocation(shaderProgram, "model");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelR));
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

