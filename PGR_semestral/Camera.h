#pragma once
#include "pgr.h"
#include "InputManager.h"

enum CameraStates
{
	freeCamera,
	staticFirst,
	staticSecond
};

class Camera
{
public:
	Camera(glm::vec3 pos);
	~Camera();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void move(const InputManager& inputManager);
	void processMouseMovement(float offset_x, float offset_y);


	CameraStates getCameraState();
	void setCameraState(CameraStates newState);

	glm::vec3 getPosition();
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	float cameraSpeed = 0.2f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float mouseSensitivity = 0.1f;
	CameraStates currentState = freeCamera;
};

