#include "Camera.h"
#include <algorithm>

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
    : position(pos), worldUp(up), yaw(yaw), pitch(pitch),
      movementSpeed(2.5f), mouseSensitivity(0.1f)
{
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    updateCameraVectors();

    // pôvodne viewMatrix nebolo inicialitovaná takže sa properne inicializovala prvý krát len pri pohybe kamery alebo pri rotácií s myšou
    // teda keď nebola inicializovaná obsahuje náhodné čísla
    // všetky vrcholy sú mimo view frustum --> nič sa nevykreslí čierna scéna
    viewMatrix = glm::lookAt(position, position + front, up);
}

void Camera::attach(ICameraObserver* observer)
{
    observers.push_back(observer);
}

void Camera::detach(ICameraObserver* observer)
{
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
}

void Camera::notify()
{
    // Aktualizuj view matrix
    viewMatrix = glm::lookAt(position, position + front, up);
    
    // Notifikuj všetkých observerov
    for (ICameraObserver* observer : observers)
    {
        observer->update(this);
    }
}

void Camera::moveForward(float deltaTime)
{
    position += front * movementSpeed * deltaTime;
    notify();
}

void Camera::moveBackward(float deltaTime)
{
    position -= front * movementSpeed * deltaTime;
    notify();
}

void Camera::moveLeft(float deltaTime)
{
    position -= right * movementSpeed * deltaTime;
    notify();
}

void Camera::moveRight(float deltaTime)
{
    position += right * movementSpeed * deltaTime;
    notify();
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;
    
    yaw += xOffset;
    pitch += yOffset;
    
    // Obmedzenie pitch (zabránime preklopeniu kamery)
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    
    updateCameraVectors();
    notify();
}

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

glm::mat4 Camera::getViewMatrix() const
{
    return viewMatrix;
}

glm::vec3 Camera::getPosition() const
{
    return position;
}

glm::vec3 Camera::getFront() const
{
    return front;
}