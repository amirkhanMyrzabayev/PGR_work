#include "SegmentedArm.h"
#include "Object.h"

SegmentedArm::SegmentedArm(const ObjectSetup& armSegment, ShaderManager& shaderManager, MeshManager& meshManager, glm::vec3 offset) :
	firstArm(std::make_unique<Object>(armSegment, shaderManager, meshManager)),
	secondArm(std::make_unique<Object>(armSegment, shaderManager, meshManager)),
	thirdArm(std::make_unique<Object>(armSegment, shaderManager, meshManager)),
	armOffset(offset)
{}

SegmentedArm::~SegmentedArm() {};

void SegmentedArm::draw(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& viewPos) {
	glm::mat4 firstArmModel = glm::mat4(1.0f);
	firstArmModel = glm::translate(firstArmModel, firstArm->getPosition());
	firstArmModel = glm::rotate(firstArmModel, glm::radians(shoulderAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	firstArmModel = glm::scale(firstArmModel, firstArm->getScale());
	firstArm->drawWithCustomModelMatrix(view, proj, viewPos, firstArmModel);

	glm::mat4 secondArmModel = firstArmModel;
	secondArmModel = glm::translate(secondArmModel, armOffset);
	secondArmModel = glm::rotate(secondArmModel, glm::radians(elbowAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	secondArm->drawWithCustomModelMatrix(view, proj, viewPos, secondArmModel);

	glm::mat4 thirdArmModel = secondArmModel;
	thirdArmModel = glm::translate(thirdArmModel, (armOffset));
	thirdArmModel = glm::rotate(thirdArmModel, glm::radians(wristAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	thirdArm->drawWithCustomModelMatrix(view, proj, viewPos, thirdArmModel);
}

void SegmentedArm::update(float deltaTime) {
	elapsedTime += deltaTime;
	if (shoulderAngle >= 45.0f) speed *= -1;
	else if (shoulderAngle <= -45.0f) speed *= -1;
	float deltaAngle = deltaTime * speed;
	shoulderAngle += deltaAngle * 2.0f;
	elbowAngle += deltaAngle * 3.0f;
	wristAngle -= (deltaAngle * 1.5f);
}

