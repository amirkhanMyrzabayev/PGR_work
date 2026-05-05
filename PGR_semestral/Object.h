#pragma once
#include <string>
#include <vector>
#include "pgr.h"
#include "mesh.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "parametry.h"

class DirectionalLight;
class PointLight;
class SpotLight;

struct ShaderLocations {
	GLint viewLoc;
	GLint projLoc;
	GLint viewPosLoc;
	GLint modelLoc;
	GLint normalMatrixLoc;
	GLint ambientLoc;
	GLint diffuseLoc;
	GLint specularLoc;
	GLint shininessLoc;
	GLint diffuseMapLoc;
	GLint hasDiffuseMapLoc;
	GLint specularMapLoc;
	GLint hasSpecularMapLoc;
	GLint texMatrixLoc;
};

class Object
{
public:
	Object(const std::string& filePath, const std::string& shaderName, ShaderManager& shaderManager, MeshManager& meshManager);
	Object(const std::string& filePath, const std::string& shaderName, ShaderManager& shaderManager, MeshManager& meshManager,
			const glm::vec3& newPos, const glm::vec3& newRotation, const glm::vec3& newScale);
	Object(const ObjectSetup& setup, ShaderManager& shaderManager, MeshManager& meshManager);
	~Object();
	virtual void draw(const glm::mat4 view, const glm::mat4& proj,
			 const glm::vec3& viewPos);

	void setPosition(const glm::vec3& newPos);
	void setRotation(const glm::vec3& newRot);
	void setScale(const glm::vec3& newScale);
	void setSRP(const glm::vec3& newPos, const glm::vec3& newRotation, const glm::vec3& newScale);
	void setTextureMatrix(const glm::mat4& matrix);

	glm::vec3 getOrientation();
	GLuint getShaderProgram();

	bool isTextureAnimated;
protected:
	ShaderLocations locations;
	Mesh* mesh;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
private:
	GLuint shaderProgram;
	glm::mat4 modelMatrix;
	glm::mat4 textureMatrix;

};

