#pragma once
#include "pgr.h"
#include "InputManager.h"

class Camera
{
public:
	Camera(glm::vec3 pos);
	~Camera();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void move(const InputManager& inputManager);
	void processMouseMovement(float offset_x, float offset_y);

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	float cameraSpeed = 0.05f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float mouseSensitivity = 0.1f;
};

