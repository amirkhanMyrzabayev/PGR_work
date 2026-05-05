#include "Object.h"
#include "ObjLoader.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"



Object::Object(const std::string& filePath, const std::string& shaderName, ShaderManager& shaderManager, MeshManager& meshManager) {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	textureMatrix = glm::mat4(1.0f);
	isTextureAnimated = false;
	shaderProgram = shaderManager.getShaderProgram(shaderName);
	locations.viewLoc = glGetUniformLocation(shaderProgram, "view");
	locations.projLoc = glGetUniformLocation(shaderProgram, "projection");
	locations.viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
	locations.modelLoc = glGetUniformLocation(shaderProgram, "model");
	locations.normalMatrixLoc = glGetUniformLocation(shaderProgram, "normalMatrix");
	locations.ambientLoc = glGetUniformLocation(shaderProgram, "matAmbient");
	locations.diffuseLoc = glGetUniformLocation(shaderProgram, "matDiffuse");
	locations.specularLoc = glGetUniformLocation(shaderProgram, "matSpecular");
	locations.shininessLoc = glGetUniformLocation(shaderProgram, "matShininess");
	locations.diffuseMapLoc = glGetUniformLocation(shaderProgram, "diffuseMap");
	locations.hasDiffuseMapLoc = glGetUniformLocation(shaderProgram, "hasDiffuseMap");
	locations.specularMapLoc = glGetUniformLocation(shaderProgram, "specularMap");
	locations.hasSpecularMapLoc = glGetUniformLocation(shaderProgram, "hasSpecularMap");
	locations.texMatrixLoc = glGetUniformLocation(shaderProgram, "texMatrix");
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
Object::Object(const ObjectSetup& setup, ShaderManager& shaderManager, MeshManager& meshManager) 
	: Object(setup.path, setup.shaderPath, shaderManager, meshManager, setup.position, setup.rotation, setup.scale)
{
}

Object::~Object() {}


void Object::setPosition(const glm::vec3& newPos) { position = newPos; }
void Object::setRotation(const glm::vec3& newRot) { rotation = newRot; }
void Object::setScale(const glm::vec3& newScale) { scale = newScale; }
glm::vec3 Object::getOrientation() { return rotation; };

void Object::setSRP(const glm::vec3& newPos, const glm::vec3& newRotation, const glm::vec3& newScale) {
	setPosition(newPos);
	setRotation(newRotation);
	setScale(newScale);
}

void Object::setTextureMatrix(const glm::mat4& matrix) { textureMatrix = matrix; }

void Object::draw(const glm::mat4 view, const glm::mat4& proj,
				 const glm::vec3& viewPos) {
	glUseProgram(shaderProgram);

	glUniformMatrix4fv(locations.viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(locations.projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(locations.texMatrixLoc, 1, GL_FALSE, glm::value_ptr(textureMatrix));

	glUniform3fv(locations.viewPosLoc, 1, glm::value_ptr(viewPos));

	glm::mat4 modelR = glm::mat4(1.0f);
	modelR = glm::translate(modelR, position);
	modelR = glm::rotate(modelR, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelR = glm::rotate(modelR, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelR = glm::rotate(modelR, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelR = glm::scale(modelR, scale);

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelR)));


	glUniformMatrix4fv(locations.modelLoc, 1, GL_FALSE, glm::value_ptr(modelR));
	glUniformMatrix3fv(locations.normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(mesh->vao);
	for (const auto& subMesh : mesh->subMeshes) {
		glUniform3fv(locations.ambientLoc, 1, glm::value_ptr(subMesh.material.ambient));
		glUniform3fv(locations.diffuseLoc, 1, glm::value_ptr(subMesh.material.diffuse));
		glUniform3fv(locations.specularLoc, 1, glm::value_ptr(subMesh.material.specular));
		glUniform1f(locations.shininessLoc, (GLfloat)subMesh.material.shininess);
		if (subMesh.material.diffuseTextureID != 0) {
			glUniform1i(locations.hasDiffuseMapLoc, 1); 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, subMesh.material.diffuseTextureID);
			glUniform1i(locations.diffuseMapLoc, 0); 
		}
		else {
			glUniform1i(locations.hasDiffuseMapLoc, 0);
		}
		if (subMesh.material.specularTextureID != 0) {
			glUniform1i(locations.hasSpecularMapLoc, 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, subMesh.material.specularTextureID);
			glUniform1i(locations.specularMapLoc, 1);
		}
		else {
			glUniform1i(locations.hasSpecularMapLoc, 0);
		}
		glDrawArrays(GL_TRIANGLES, subMesh.startIndex, subMesh.numVertices);
	}
	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

