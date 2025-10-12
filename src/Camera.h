#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "ICameraObserver.h"

class Camera
{
private:

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;


    float yaw;
    float pitch;


    float movementSpeed;
    float mouseSensitivity;


    glm::mat4 viewMatrix;


    std::vector<ICameraObserver*> observers;


    void updateCameraVectors();

    void notify();

public:
    // Konštruktor
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f);

    // Observer pattern metódy
    void attach(ICameraObserver* observer);
    void detach(ICameraObserver* observer);

    // Pohyb kamery (WSAD)
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);

    // Rotácia kamery myšou
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

    // Gettery
    glm::mat4 getViewMatrix() const;
    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;
};

#endif