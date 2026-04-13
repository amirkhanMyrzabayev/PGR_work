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
		glm::vec3 rotatedFront = glm::vec3(trackballRotation * glm::vec4(front, 0.0f));
		glm::vec4 rotatedUp4D = trackballRotation * glm::vec4(up, 0.0f);
		glm::vec3 rotatedUp = glm::vec3(rotatedUp4D);
		return glm::lookAt(position, position + rotatedFront, rotatedUp);
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
	glm::vec3 currentFront = glm::vec3(trackballRotation * glm::vec4(front, 0.0f));
	glm::vec3 currentUp = glm::vec3(trackballRotation * glm::vec4(up, 0.0f));

	if (inputManager.specialKeys[GLUT_KEY_UP]) {
		position += currentUp * cameraSpeed;
	}
	if (inputManager.specialKeys[GLUT_KEY_DOWN]) {
		position -= currentUp * cameraSpeed;
	}
	if (inputManager.keys['w']) {
		position += currentFront * cameraSpeed;
	}
	if (inputManager.keys['s']) {
		position -= currentFront * cameraSpeed;
	}
	if (inputManager.keys['a']) {
		position -= glm::normalize(glm::cross(currentFront, currentUp)) * cameraSpeed;
	}
	if (inputManager.keys['d']) {
		position += glm::normalize(glm::cross(currentFront, currentUp)) * cameraSpeed;
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
		trackballRotation = glm::mat4(1.0f);
		pitch = glm::degrees(glm::asin(-front.y));
		yaw = glm::degrees(glm::atan(front.z, front.x));
		//front = glm::normalize(front);
		//if (std::abs(front.y + 1.0f) < 0.001f) {
		//	front.z += 0.0000001f;
		//}
		//glm::vec3 sFront = glm::normalize(STATIC_CAMERAS[currentState - 1].front); 
		//glm::vec3 sUp = glm::normalize(STATIC_CAMERAS[currentState - 1].up);
		//glm::quat q = 
	}
	currentState = newState;
}


glm::vec3 Camera::mapToSphere(float x, float y, float screenWidth, float screenHeight) {
	float trackballRadius = 0.8f;
	float normX = (x * 2.0f - screenWidth) / screenWidth;
	float normY = (screenHeight - 2.0f * y) / screenHeight;
	float lengthSquared = (normX * normX) + (normY * normY);
	float dist = std::sqrt(lengthSquared);
	float z = 0.0f;
	if (dist < trackballRadius * 0.70710678f) {
		z = std::sqrt(trackballRadius * trackballRadius - lengthSquared);
	}
	else {
		z = trackballRadius * trackballRadius / (2.0f * dist);
	}
	return glm::normalize(glm::vec3(normX, normY, z));
}	

void Camera::startTrackball(float x, float y, float screenWidth, float screenHeight) {
	lastTrackballPos = mapToSphere(x, y, screenWidth, screenHeight);
}

void Camera::processTrackballMovement(float x, float y, float screenWidth, float screenHeight) {
	glm::vec3 currentPos = mapToSphere(x, y, screenWidth, screenHeight);
	float angle = glm::acos(glm::dot(lastTrackballPos, currentPos));
	glm::vec3 axis = glm::cross(lastTrackballPos, currentPos);
	if (angle > 0.0001f && glm::length(axis) > 0.0001f) {
		glm::mat4 currentRotation = glm::rotate(glm::mat4(1.0f), angle, axis);
		trackballRotation = currentRotation * trackballRotation;
	}
	lastTrackballPos = currentPos;
}






