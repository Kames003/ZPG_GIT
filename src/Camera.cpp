#include "Camera.h"
#include <algorithm>
#include <cstdio>
#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch, bool movable)
    : position(pos), worldUp(up), yaw(yaw), pitch(pitch),
      movementSpeed(2.5f), mouseSensitivity(0.1f),
      firstMouse(true), lastX(400.0), lastY(300.0),
      isMovable(movable)  // ← NOVÝ
{
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    updateCameraVectors();
    updateViewMatrix();
}
// ================== OBSERVER PATTERN ==================

void Camera::attachObserver(ICameraObserver* observer)
{
    observerCollection.push_back(observer);
}

void Camera::detachObserver(ICameraObserver* observer)
{
    observerCollection.erase(
        std::remove(observerCollection.begin(), observerCollection.end(), observer),
        observerCollection.end()
    );
}

void Camera::notifyObservers()
{
    // PULL pattern - observeri si sami vytiahnu view matrix cez getViewMatrix()
    for (ICameraObserver* observer : observerCollection)
    {
        observer->update(this);
    }
}

// ================== INPUT PROCESSING ==================

void Camera::processMouseInput(double xpos, double ypos)
{
    if (!isMovable) return;  // ← Statická kamera - bez otáčania

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
    notifyObservers();
}



// ================== CAMERA VECTORS UPDATE ==================

void Camera::updateCameraVectors()
{
    // Výpočet nového front vektora
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Prepočítaj right a up vektory
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::updateViewMatrix()
{
    // Prepočet view matrix - interná zodpovednosť kamery
    viewMatrix = glm::lookAt(position, position + front, up);
}

void Camera::moveForward(float deltaTime)
{
    if (!isMovable) return;  // ← Statická kamera sa neposunie

    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    position += forward * movementSpeed * deltaTime;
    position.y = 0.3f;  // Ostáva nad zemou

    updateViewMatrix();
    notifyObservers();
}

void Camera::moveBackward(float deltaTime)
{
    if (!isMovable) return;

    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    position -= forward * movementSpeed * deltaTime;
    position.y = 0.3f;

    updateViewMatrix();
    notifyObservers();
}

void Camera::moveLeft(float deltaTime)
{
    if (!isMovable) return;

    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    glm::vec3 rightVec = glm::normalize(glm::cross(forward, worldUp));
    position -= rightVec * movementSpeed * deltaTime;
    position.y = 0.3f;

    updateViewMatrix();
    notifyObservers();
}

void Camera::moveRight(float deltaTime)
{
    if (!isMovable) return;

    glm::vec3 forward = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
    glm::vec3 rightVec = glm::normalize(glm::cross(forward, worldUp));
    position += rightVec * movementSpeed * deltaTime;
    position.y = 0.3f;

    updateViewMatrix();
    notifyObservers();
}

