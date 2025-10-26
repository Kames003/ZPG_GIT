#include "Camera.h"
#include <algorithm>
#include <cstdio>
#include <GLFW/glfw3.h>

// ========================================
// CONSTRUCTOR
// ========================================
Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch, bool movable)
    : position(pos), worldUp(up), yaw(yaw), pitch(pitch),
      movementSpeed(2.5f), mouseSensitivity(0.1f),
      firstMouse(true), lastX(400.0), lastY(300.0),
      isMovable(movable),
      fov(45.0f),
      aspectRatio(4.0f / 3.0f),
      nearPlane(0.1f),
      farPlane(100.0f),
      needsNotification(false)  // ✅ Inicializácia flagu
{
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    updateCameraVectors();
    updateViewMatrix();
    updateProjectionMatrix();
}

// ========================================
// MOUSE INPUT
// ========================================
void Camera::processMouseInput(double xpos, double ypos)
{
    if (!isMovable) return;  // Statická kamera - bez otáčania

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    double xOffset = xpos - lastX;
    double yOffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateCameraVectors();
    updateViewMatrix();
    notifyAll();
}

// ========================================
// UPDATE CAMERA VECTORS
// ========================================
void Camera::updateCameraVectors()
{
    // new front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // right a up vectors
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

// ========================================
// UPDATE VIEW MATRIX
// ========================================
void Camera::updateViewMatrix()
{
    // Recalculate view matrix - internal responsibility of the camera
    viewMatrix = glm::lookAt(position, position + front, up);
}

// ========================================
// UPDATE PROJECTION MATRIX
// ========================================
void Camera::updateProjectionMatrix()
{
    projectionMatrix = glm::perspective(
        glm::radians(fov),
        aspectRatio,
        nearPlane,
        farPlane
    );
}

// ========================================
// ✅ SET ASPECT RATIO (s odloženou notifikáciou)
// ========================================
void Camera::setAspectRatio(float aspect)
{
    aspectRatio = aspect;
    updateProjectionMatrix();
    needsNotification = true;  // ← Odložená notifikácia!

    printf("Camera: Aspect ratio changed to %.2f (notification pending)\n", aspect);
}

// ========================================
// ✅ SET FOV (s odloženou notifikáciou)
// ========================================
void Camera::setFOV(float newFov)
{
    fov = newFov;
    updateProjectionMatrix();
    needsNotification = true;  // ← Odložená notifikácia!

    printf("Camera: FOV changed to %.1f° (notification pending)\n", fov);
}

// ========================================
// ✅ SET PROJECTION PLANES
// ========================================
void Camera::setProjectionPlanes(float near, float far)
{
    nearPlane = near;
    farPlane = far;
    updateProjectionMatrix();
    needsNotification = true;
}

// ========================================
// ✅ FLUSH PENDING NOTIFICATIONS
// ========================================
void Camera::flushPendingNotifications()
{
    if (!needsNotification) return;  // Early exit

    notifyAll();
    needsNotification = false;
    printf("Camera: Observers notified ✓\n");
}

// ========================================
// MOVEMENT METHODS
// ========================================
void Camera::moveForward(float deltaTime)
{
    if (!isMovable) return;  // Statická kamera sa neposunie

    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    position += forward * movementSpeed * deltaTime;
    position.y = 0.3f;  // Ostáva nad zemou

    updateViewMatrix();
    notifyAll();
}

void Camera::moveBackward(float deltaTime)
{
    if (!isMovable) return;

    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    position -= forward * movementSpeed * deltaTime;
    position.y = 0.3f;

    updateViewMatrix();
    notifyAll();
}

void Camera::moveLeft(float deltaTime)
{
    if (!isMovable) return;

    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    glm::vec3 rightVec = glm::normalize(glm::cross(forward, worldUp));
    position -= rightVec * movementSpeed * deltaTime;
    position.y = 0.3f;

    updateViewMatrix();
    notifyAll();
}

void Camera::moveRight(float deltaTime)
{
    if (!isMovable) return;

    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    glm::vec3 rightVec = glm::normalize(glm::cross(forward, worldUp));
    position += rightVec * movementSpeed * deltaTime;
    position.y = 0.3f;

    updateViewMatrix();
    notifyAll();
}

// ========================================
// SET POSITION
// ========================================
void Camera::setPosition(const glm::vec3& pos)
{
    position = pos;
    updateViewMatrix();
    notifyAll();
}

// ========================================
// LOOK AT
// ========================================
void Camera::lookAt(const glm::vec3& target)
{
    glm::vec3 direction = glm::normalize(target - position);

    // Vypočítaj yaw a pitch z direction vektora
    pitch = glm::degrees(asin(direction.y));
    yaw = glm::degrees(atan2(direction.z, direction.x));

    updateCameraVectors();
    updateViewMatrix();
    notifyAll();
}