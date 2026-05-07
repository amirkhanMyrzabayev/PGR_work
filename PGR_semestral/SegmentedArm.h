#pragma once
#include "parametry.h"
#include "ShaderManager.h"
#include "MeshManager.h"

class Object;

class SegmentedArm 
{
public:
	SegmentedArm(const ObjectSetup& armSegment, ShaderManager& shaderManager, MeshManager& meshManager, glm::vec3 offset);
	~SegmentedArm();
	void draw(const glm::mat4& view, const glm::mat4& proj,
				const glm::vec3& viewPos);
	void update(float deltaTime);

private:
	std::unique_ptr<Object> firstArm;
	std::unique_ptr<Object> secondArm;
	std::unique_ptr<Object> thirdArm;
	glm::vec3 armOffset;
	float elapsedTime = 0.0f;
	float shoulderAngle = 0.0f;
	float elbowAngle = 0.0f;
	float wristAngle = 0.0f;
	float speed = 6.0f;
};

