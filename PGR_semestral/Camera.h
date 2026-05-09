#pragma once
#include "pgr.h"
#include "InputManager.h"

enum CameraStates
{
	freeCamera,
	staticFirst,
	staticSecond,
	splineCamera,
	movingWithObject
};

class Camera
{
public:
	Camera(glm::vec3 pos, const std::vector<glm::vec3>& animationPath);
	~Camera();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	void move(const InputManager& inputManager, std::vector<std::pair<glm::vec3, float>>& collisionCircles, float deltaTime);
	void processMouseMovement(float offset_x, float offset_y);


	CameraStates getCameraState();
	void setCameraState(CameraStates newState);
	void setPosition(glm::vec3 newPosition) { position = newPosition; };
	glm::vec3 getPosition();
	void moveWithObject(glm::vec3 objectPosition, float objectYaw);

	

private:
	glm::vec3 checkBounds(glm::vec3 newPosition);
	bool checkCollision(std::vector<std::pair<glm::vec3, float>>& collisionCircles, glm::vec3 newPosition);
	void animate();
	glm::vec3 getSplinePosition(float totalTime, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	std::vector<glm::vec3> path;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	float cameraSpeed = 0.2f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float mouseSensitivity = 0.1f;
	float elapsedTime = 0.0f;
	CameraStates currentState = freeCamera;
};

