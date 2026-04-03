#pragma once
#include <string>
#include <vector>
#include "pgr.h"
#include "mesh.h"
#include "MeshManager.h"



class Object
{
public:
	Object(const std::string& filePath, GLuint _shaderProgram, MeshManager& meshManager);
	Object(const std::string& filePath, GLuint _shaderProgram, MeshManager& meshManager, const glm::vec3& newPos, const glm::vec3& newRotation, const glm::vec3& newScale);
	~Object();
	void draw();

	void setPosition(const glm::vec3& newPos);
	void setRotation(const glm::vec3& newRot);
	void setScale(const glm::vec3& newScale);
	void setSRP(const glm::vec3& newPos, const glm::vec3& newRotation, const glm::vec3& newScale);
private:
	GLuint shaderProgram;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 modelMatrix;
	Mesh* mesh;
};

