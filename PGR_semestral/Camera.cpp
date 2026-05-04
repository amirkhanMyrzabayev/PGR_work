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

void Camera::move(const InputManager& inputManager) {
	if (currentState != freeCamera) return;
	

	if (inputManager.specialKeys[GLUT_KEY_UP]) {
		position += up * cameraSpeed;
	}
	if (inputManager.specialKeys[GLUT_KEY_DOWN]) {
		position -= up * cameraSpeed;
	}
	if (inputManager.keys['w']) {
		position += front * cameraSpeed;
	}
	if (inputManager.keys['s']) {
		position -= front * cameraSpeed;
	}
	if (inputManager.keys['a']) {
		position -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
	}
	if (inputManager.keys['d']) {
		position += glm::normalize(glm::cross(front, up)) * cameraSpeed;
	}
	return;
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






