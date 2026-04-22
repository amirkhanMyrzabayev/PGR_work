#include "Object.h"
#include "ObjLoader.h"


Object::Object(const std::string& filePath, const std::string& shaderName, ShaderManager& shaderManager, MeshManager& meshManager) {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	shaderProgram = shaderManager.getShaderProgram(shaderName);
	this->mesh = meshManager.getMesh(filePath, shaderProgram);
}

Object::Object(const std::string& filePath, const std::string& shaderName,
				ShaderManager& shaderManager,
				MeshManager& meshManager,
				const glm::vec3& newPos, 
				const glm::vec3& newRotation, 
				const glm::vec3& newScale) : Object(filePath, shaderName, shaderManager, meshManager){
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

void Object::draw(const glm::mat4 view, const glm::mat4& proj,
	const glm::vec3& lightPos, const glm::vec3& viewPos) {
	glUseProgram(shaderProgram);
	GLint viewLocation = glGetUniformLocation(shaderProgram, "view");
	GLint projLocation = glGetUniformLocation(shaderProgram, "projection");
	GLint lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
	GLint viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");


	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));
	glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPos));
	glUniform3fv(viewPosLocation, 1, glm::value_ptr(viewPos));

	glm::mat4 modelR = glm::mat4(1.0f);
	modelR = glm::translate(modelR, position);
	modelR = glm::rotate(modelR, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelR = glm::rotate(modelR, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelR = glm::rotate(modelR, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelR = glm::scale(modelR, scale);

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelR)));
	GLint modelLocation = glGetUniformLocation(shaderProgram, "model");
	GLint normalMatrixLocation = glGetUniformLocation(shaderProgram, "normalMatrix");

	GLint ambientLocation = glGetUniformLocation(shaderProgram, "matAmbient");
	GLint diffuseLocation = glGetUniformLocation(shaderProgram, "matDiffuse");
	GLint specularlLocation = glGetUniformLocation(shaderProgram, "matSpecular");
	GLint shininessLocation = glGetUniformLocation(shaderProgram, "matShininess");

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelR));
	glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(mesh->vao);
	for (const auto& subMesh : mesh->subMeshes) {
		glUniform3fv(ambientLocation, 1, glm::value_ptr(subMesh.material.ambient));
		glUniform3fv(diffuseLocation, 1, glm::value_ptr(subMesh.material.diffuse));
		glUniform3fv(specularlLocation, 1, glm::value_ptr(subMesh.material.specular));
		glUniform1f(shininessLocation, (GLfloat)subMesh.material.shininess);
		glDrawArrays(GL_TRIANGLES, subMesh.startIndex, subMesh.numVertices);
	}
	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

