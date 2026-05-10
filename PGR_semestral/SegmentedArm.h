/**

    @file      SegmentedArm.h
    @brief     Wrapper class for 3 Objects that will have hierarchical animation.
    @details   ~
    @author    Amirkhan Myrzabayev
    @date      10.05.2026
    @copyright © Amirkhan Myrzabayev, 2026. All right reserved.

**/
#pragma once

#include "parametry.h"
#include "ShaderManager.h"
#include "MeshManager.h"

class Object;

/// @brief A 3-part connected arm (shoulder, elbow, wrist) that animates automatically.
class SegmentedArm
{
public:
	/// @brief Creates the three parts of the arm and sets the distance between their joints.
	/// @param armSegment Settings used to load the 3D model for all three parts of the arm.
	/// @param shaderManager Reference to get the shader program.
	/// @param meshManager Reference to get the 3D mesh.
	/// @param offset The 3D distance from one joint to the next.
	SegmentedArm(const ObjectSetup& armSegment, ShaderManager& shaderManager, MeshManager& meshManager, glm::vec3 offset);

	~SegmentedArm();

	/// @brief Calculates the chained math (hierarchy) and draws the three arm segments.
	/// @param view The camera's view matrix.
	/// @param proj The camera's projection matrix.
	/// @param viewPos The 3D position of the camera.
	void draw(const glm::mat4& view, const glm::mat4& proj,
		const glm::vec3& viewPos);

	/// @brief Swings the arm joints back and forth based on time.
	/// @param deltaTime Time passed since the last frame.
	void update(float deltaTime);

private:
	std::unique_ptr<Object> firstArm;
	std::unique_ptr<Object> secondArm;
	std::unique_ptr<Object> thirdArm;
	glm::vec3 armOffset;         ///< The distance added between each connected arm part.
	float elapsedTime = 0.0f;
	float shoulderAngle = 0.0f;  ///< Current rotation of the first arm piece.
	float elbowAngle = 0.0f;     ///< Current rotation of the second arm piece.
	float wristAngle = 0.0f;     ///< Current rotation of the third arm piece.
	float speed = 6.0f;          ///< How fast the arm swings back and forth.
};