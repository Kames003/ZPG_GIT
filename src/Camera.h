#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Subject.h"  // ← ZMENA: Namiesto ICameraObserver

class Camera : public Subject
// no need for own observerCollection

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

    bool firstMouse;
    double lastX, lastY;
    bool isMovable;

    //  std::vector<ICameraObserver*> observerCollection je v Subject


    void updateCameraVectors();
    void updateViewMatrix();

public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f,
           bool movable = false);

    //  používame attach(), detach() zo Subject!

    void processMouseInput(double xpos, double ypos);

    // Gettery pre PULL pattern
    glm::mat4 getViewMatrix() const { return viewMatrix; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }

    // Movement metódy
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);

    bool getIsMovable() const { return isMovable; }
    void resetMouseState() { firstMouse = true; }
};

#endif