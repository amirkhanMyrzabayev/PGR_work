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

/// @brief Stores OpenGL memory locations for shader variables so we don't have to look them up every frame.
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
	GLint alphaLoc;
	GLint elapsedTimeLoc;
};

/// @brief The base class for all 3D things in your scene. Handles loading, transforming, and drawing.
class Object
{
public:
	/// @brief Creates an object, loading its mesh and preparing its shader.
	/// @param filePath Path to the 3D model file (or a special keyword like "HARD").
	/// @param shaderName Base name of the shader files.
	/// @param shaderManager Reference to the manager to get the shader program.
	/// @param meshManager Reference to the manager to get the 3D mesh.
	Object(const std::string& filePath, const std::string& shaderName, ShaderManager& shaderManager, MeshManager& meshManager);

	/// @brief Creates an object and immediately sets its starting position, rotation, and scale.
	/// @param filePath Path to the 3D model file.
	/// @param shaderName Base name of the shader files.
	/// @param shaderManager Reference to the manager to get the shader program.
	/// @param meshManager Reference to the manager to get the 3D mesh.
	/// @param newPos The starting 3D position.
	/// @param newRotation The starting 3D rotation angles.
	/// @param newScale The starting 3D size multipliers.
	Object(const std::string& filePath, const std::string& shaderName, ShaderManager& shaderManager, MeshManager& meshManager,
		const glm::vec3& newPos, const glm::vec3& newRotation, const glm::vec3& newScale);

	/// @brief Creates an object using a pre-filled setup struct.
	/// @param setup Struct containing all the needed paths and transform settings.
	/// @param shaderManager Reference to the manager to get the shader program.
	/// @param meshManager Reference to the manager to get the 3D mesh.
	Object(const ObjectSetup& setup, ShaderManager& shaderManager, MeshManager& meshManager);

	~Object();

	/// @brief Calculates the final model matrix and draws the object on the screen.
	/// @param view The camera's view matrix.
	/// @param proj The camera's projection matrix.
	/// @param viewPos The 3D position of the camera (used to calculate shiny specular reflections).
	virtual void draw(const glm::mat4& view, const glm::mat4& proj,
		const glm::vec3& viewPos);

	/// @brief Updates object logic (like sending elapsed time to the shader for animations).
	/// @param deltaTime Time passed since the last frame.
	virtual void update(float deltaTime);

	/// @brief Draws the object using a specific model matrix instead of calculating it from position/rotation/scale.
	/// @param view The camera's view matrix.
	/// @param proj The camera's projection matrix.
	/// @param viewPos The 3D position of the camera.
	/// @param model The custom 4x4 model matrix to use.
	void drawWithCustomModelMatrix(const glm::mat4& view, const glm::mat4& proj,
		const glm::vec3& viewPos, glm::mat4& model);

	/// @param newPos The new 3D position.
	void setPosition(const glm::vec3& newPos);

	/// @param newRot The new 3D rotation values (pitch, yaw, roll).
	void setRotation(const glm::vec3& newRot);

	/// @param newScale The new 3D scale multipliers.
	void setScale(const glm::vec3& newScale);

	/// @brief Sets position, rotation, and scale all at once.
	/// @param newPos The new 3D position.
	/// @param newRotation The new 3D rotation values.
	/// @param newScale The new 3D scale multipliers.
	void setSRP(const glm::vec3& newPos, const glm::vec3& newRotation, const glm::vec3& newScale);

	/// @param matrix The 4x4 matrix used to slide or scale the texture over the object.
	void setTextureMatrix(const glm::mat4& matrix);

	/// @brief Attaches a point light to this object so they can interact or move together.
	/// @param newLight Pointer to the point light.
	void setLight(PointLight* newLight);

	/// @brief Attaches a spotlight to this object.
	/// @param newLight Pointer to the spotlight.
	void setLight(SpotLight* newLight);

	/// @brief Toggles any attached lights on or off.
	void switchLight();

	GLuint getShaderProgram() { return shaderProgram; };

	glm::vec3 getPosition() { return position; };
	glm::vec3 getOrientation() { return rotation; };
	glm::vec3 getScale() { return scale; };

	bool hasLight() { return haveLight; };

	/// @param newId The integer ID to assign to this object.
	void setId(int newId) { id = newId; };
	int getId() { return id; };

	bool isTextureAnimated; ///< Flag to check if the texture should be moving over time.

protected:
	ShaderLocations locations;
	Mesh* mesh;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	int id = 0;

private:
	bool haveLight = false;
	PointLight* pointLight = nullptr;
	SpotLight* spotLight = nullptr;
	GLuint shaderProgram;
	glm::mat4 modelMatrix;
	glm::mat4 textureMatrix;
};