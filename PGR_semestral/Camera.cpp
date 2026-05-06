#include "Camera.h"
#include "parametry.h"
#include <glm/gtc/quaternion.hpp>

Camera::Camera(glm::vec3 pos) : position(pos) {
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {
	switch (currentState)
	{
	case freeCamera:
		//glm::vec4 position4D = glm::vec4(position, 1.0f);
		//glm::vec4 rotatedEye4D = trackballRotation * glm::vec4(position, 1.0f);
		//glm::vec3 rotatedEye = glm::vec3(rotatedEye4D);
	{
		//glm::vec4 baseFront = glm::vec4(front, 0.0f);

		return glm::lookAt(position, position + front, up);
	}
	case staticFirst:
		return glm::lookAt(STATIC_CAMERAS[0].position,
			STATIC_CAMERAS[0].position + STATIC_CAMERAS[0].front, STATIC_CAMERAS[0].up);
	case staticSecond:
		return glm::lookAt(STATIC_CAMERAS[1].position,
			STATIC_CAMERAS[1].position + STATIC_CAMERAS[1].front, STATIC_CAMERAS[1].up);
	default:
		break;
	}
}

glm::vec3 Camera::getPosition() {
	switch (currentState)
	{
	case freeCamera:
		return position;
	case staticFirst:
		return STATIC_CAMERAS[0].position;
	case staticSecond:
		return STATIC_CAMERAS[1].position;
	default:
		break;
	}
}

glm::mat4 Camera::getProjectionMatrix() {
	return glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
}

void Camera::move(const InputManager& inputManager, std::vector<std::pair<glm::vec3, float>>& collisionCircles) {
	if (currentState != freeCamera) return;
	glm::vec3 nextPosition = position;

	if (inputManager.specialKeys[GLUT_KEY_UP]) {
		nextPosition += up * cameraSpeed;
	}
	if (inputManager.specialKeys[GLUT_KEY_DOWN]) {
		nextPosition -= up * cameraSpeed;
	}
	if (inputManager.keys['w']) {
		nextPosition += front * cameraSpeed;
	}
	if (inputManager.keys['s']) {
		nextPosition -= front * cameraSpeed;
	}
	if (inputManager.keys['a']) {
		nextPosition -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
	}
	if (inputManager.keys['d']) {
		nextPosition += glm::normalize(glm::cross(front, up)) * cameraSpeed;
	}
	if (checkCollision(collisionCircles, nextPosition)) return;
	position = checkBounds(nextPosition);
	return;
}

glm::vec3 Camera::checkBounds(glm::vec3 newPosition) {
	glm::vec3 retPosition = newPosition;
	if (newPosition.x >= MAX_X) retPosition.x = MAX_X;
	else if (newPosition.x <= MIN_X) retPosition.x = MIN_X;
	if (newPosition.y >= MAX_Y) retPosition.y = MAX_Y;
	else if (newPosition.y <= MIN_Y) retPosition.y = MIN_Y;
	if (newPosition.z >= MAX_Z) retPosition.z = MAX_Z;
	else if (newPosition.z <= MIN_Z) retPosition.z = MIN_Z;
	return retPosition;
}

bool Camera::checkCollision(std::vector<std::pair<glm::vec3, float>>& collisionCircles, glm::vec3 newPosition) {
	float distance;
	for (auto const& circle : collisionCircles) {
		distance = glm::distance(circle.first, newPosition);
		if (distance <= circle.second) return true;
	}
	return false;
}



void Camera::processMouseMovement(float offset_x, float offset_y) {
	offset_x *= mouseSensitivity;
	offset_y *= mouseSensitivity;
	yaw += offset_x;
	pitch += offset_y;
	if (pitch > 89.0f) pitch = 89.0f;
	else if (pitch < -89.0f) pitch = -89.0f;
	using glm::cos;
	using glm::sin;
	using glm::radians;
	front.x = cos(radians(yaw)) * cos(radians(pitch));	
	front.y = -sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	front = glm::normalize(front);
}

CameraStates Camera::getCameraState() {
	return currentState;
}

void Camera::setCameraState(CameraStates newState) {
	if (newState == freeCamera && currentState != freeCamera) {
		position = STATIC_CAMERAS[currentState - 1].position;
		front = STATIC_CAMERAS[currentState - 1].front;
		up = STATIC_CAMERAS[currentState - 1].up;
		pitch = glm::degrees(glm::asin(-front.y));
		yaw = glm::degrees(glm::atan(front.z, front.x));
	}
	currentState = newState;
}






