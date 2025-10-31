#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Subject.h"

class Camera : public Subject
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
    glm::mat4 projectionMatrix;
    float fov; // verticalos uhlos
    float aspectRatio;
    float nearPlane;
    float farPlane;

    bool firstMouse;
    double lastX, lastY;
    bool isMovable;
    bool needsNotification;

    void updateCameraVectors();
    void updateViewMatrix();
    void updateProjectionMatrix();

public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f,
           bool movable = false);

    // Gettery
    glm::mat4 getProjectionMatrix() const { return projectionMatrix; }
    float getFOV() const { return fov; }
    float getAspectRatio() const { return aspectRatio; }
    glm::mat4 getViewMatrix() const { return viewMatrix; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }
    bool getIsMovable() const { return isMovable; }

    // Nenotifikuje hneď, len nastaví flag
    void setAspectRatio(float aspect);
    void setFOV(float newFov);
    void setProjectionPlanes(float near, float far);

    // Bezpečná notifikácia mimo render loop
    void flushPendingNotifications();

    void processMouseInput(double xpos, double ypos);
    void setPosition(const glm::vec3& pos);
    void lookAt(const glm::vec3& target);

    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);

    void resetMouseState() { firstMouse = true; }
};

#endif