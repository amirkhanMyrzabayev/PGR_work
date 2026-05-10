#pragma once
#include "pgr.h"
#include "InputManager.h"

/// @brief Defines the current behavior mode of the camera.
enum CameraStates
{
    freeCamera,       ///< Player can move freely using keyboard and mouse.
    staticFirst,      ///< First fixed camera view (loaded from parameters).
    staticSecond,     ///< Second fixed camera view (loaded from parameters).
    splineCamera,     ///< Camera moves automatically along a predefined curve.
    movingWithObject  ///< Camera is locked to follow a specific scene object.
};

/// @brief Handles 3D viewing, movement, collision detection, and spline animation.
class Camera
{
public:
    /// @brief Sets up the camera's starting point and animation route.
    /// @param pos Starting 3D position.
    /// @param animationPath List of points for the camera to follow when in spline mode.
    Camera(glm::vec3 pos, const std::vector<glm::vec3>& animationPath);

    ~Camera();

    /// @brief Calculates the view matrix depending on the current camera state.
    /// @return The 4x4 view matrix for shaders.
    glm::mat4 getViewMatrix();

    /// @brief Calculates the perspective projection matrix.
    /// @return The 4x4 projection matrix.
    glm::mat4 getProjectionMatrix();

    /// @brief Updates the camera position based on keyboard input or spline animation.
    /// @param inputManager Reference to the input system to check key presses.
    /// @param collisionCircles List of objects (position and radius) to avoid walking through.
    /// @param deltaTime Time passed since the last frame, used to make movement smooth.
    void move(const InputManager& inputManager, std::vector<std::pair<glm::vec3, float>>& collisionCircles, float deltaTime);

    /// @brief Updates where the camera is looking based on mouse movement.
    /// @param offset_x Horizontal mouse movement.
    /// @param offset_y Vertical mouse movement.
    void processMouseMovement(float offset_x, float offset_y);

    /// @return The current mode of the camera.
    CameraStates getCameraState();

    /// @brief Changes the camera mode (e.g., switches from free flying to a static view).
    /// @param newState The new mode to switch to.
    void setCameraState(CameraStates newState);

    /// @param newPosition The new 3D position to instantly move the camera to.
    void setPosition(glm::vec3 newPosition) { position = newPosition; };

    /// @return The current 3D position of the camera.
    glm::vec3 getPosition();

    /// @brief Locks the camera to match an object's location and rotation.
    /// @param objectPosition The 3D position of the object being followed.
    /// @param objectYaw The rotation angle (yaw) of the object so the camera looks the same way.
    void moveWithObject(glm::vec3 objectPosition, float objectYaw);

private:
    /// @brief Prevents the camera from flying outside the map limits defined in parameters.
    /// @param newPosition The planned next position.
    /// @return The corrected position kept inside the map borders.
    glm::vec3 checkBounds(glm::vec3 newPosition);

    /// @brief Checks if the camera's next step will hit a collision circle.
    /// @param collisionCircles List of positions and radiuses.
    /// @param newPosition The planned next position.
    /// @return True if a collision happens, false if the path is clear.
    bool checkCollision(std::vector<std::pair<glm::vec3, float>>& collisionCircles, glm::vec3 newPosition);

    /// @brief Moves the camera one step further along its spline path.
    void animate();

    /// @brief Calculates a point on a curve for smooth animation.
    /// @param totalTime Current progress time on the line segment (from 0.0 to 1.0).
    /// @param p0 Control point 0.
    /// @param p1 Control point 1 (start of the current segment).
    /// @param p2 Control point 2 (end of the current segment).
    /// @param p3 Control point 3.
    /// @return The calculated 3D position on the curve.
    glm::vec3 getSplinePosition(float totalTime, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

    std::vector<glm::vec3> path;
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float cameraSpeed = 0.2f;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float mouseSensitivity = 0.1f;
    float elapsedTime = 0.0f; ///< Tracks time specifically for moving along the spline path.
    CameraStates currentState = freeCamera;
};