#include "Camera.h"
#include "parametry.h"

Camera::Camera(glm::vec3 pos, const std::vector<glm::vec3>& animationPath) : position(pos), path(animationPath) {
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {
	switch (currentState)
	{
	case staticFirst:
		return glm::lookAt(STATIC_CAMERAS[0].position,
			STATIC_CAMERAS[0].position + STATIC_CAMERAS[0].front, STATIC_CAMERAS[0].up);
	case staticSecond:
		return glm::lookAt(STATIC_CAMERAS[1].position,
			STATIC_CAMERAS[1].position + STATIC_CAMERAS[1].front, STATIC_CAMERAS[1].up);
	default:
		return glm::lookAt(position, position + front, up);
	}
}

glm::vec3 Camera::getPosition() {
	switch (currentState)
	{
	case staticFirst:
		return STATIC_CAMERAS[0].position;
	case staticSecond:
		return STATIC_CAMERAS[1].position;
	default:
		return position;
	}
}

glm::mat4 Camera::getProjectionMatrix() {
	return glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
}

glm::vec3 Camera::getSplinePosition(float totalTime, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	float t2 = totalTime * totalTime;
	float t3 = t2 * totalTime;
	
	return 0.5f * (
		(2.0f * p1) +
		(-p0 + p2) * totalTime +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
		(-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
		);
}
void Camera::animate() {
	float pathTime = elapsedTime * 0.5f;
	int segment = static_cast<int>(pathTime) % (path.size() - 3);
	float t = pathTime - floor(pathTime);
	position = getSplinePosition(t, path[segment], path[(segment + 1)], path[(segment + 2)], path[(segment + 3)]);
}

void Camera::move(const InputManager& inputManager, std::vector<std::pair<glm::vec3, float>>& collisionCircles, float deltaTime) {
	if (currentState == splineCamera) {
		elapsedTime += deltaTime;
		animate();
		return;
	}
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
	if (newState == freeCamera && (currentState == staticFirst || currentState == staticSecond)) {
		position = STATIC_CAMERAS[currentState - 1].position;
		front = glm::normalize(STATIC_CAMERAS[currentState - 1].front);
		up = glm::normalize(STATIC_CAMERAS[currentState - 1].up);
		pitch = glm::degrees(glm::asin(-front.y));
		yaw = glm::degrees(glm::atan(front.z, front.x));
	}
	currentState = newState;
}


void Camera::moveWithObject(glm::vec3 objectPosition, float objectYaw) {
	position = objectPosition;
	pitch = 0.0f;
	yaw = -glm::degrees(objectYaw) + 90.0f;
	using glm::radians;
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = -sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));
	front = glm::normalize(front);

}






