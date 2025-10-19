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
    // Pozícia a orientácia
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;

    // View matrix cache
    glm::mat4 viewMatrix;

    // Observer pattern
    std::vector<ICameraObserver*> observerCollection;

    // Mouse handling
    bool firstMouse;
    double lastX, lastY;


    bool isMovable;  // len flag true = cameraDynamic, false = cameraStatic

    // Privátne pomocné metódy
    void updateCameraVectors();
    void updateViewMatrix();

public:
    // Konštruktor
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f,
           bool movable = false);  // ← NOVÝ parameter

    // Observer pattern
    void attachObserver(ICameraObserver* observer);
    void detachObserver(ICameraObserver* observer);
    void notifyObservers();  // ← ZMENA: PUBLIC! (bolo private)

    // Input processing (volanú z Controller)
    void processKeyboardInput(int key, int action);
    void processMouseInput(double xpos, double ypos);

    // Gettery pre PULL pattern
    glm::mat4 getViewMatrix() const { return viewMatrix; }
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }

    // ← NOVÉ: Movement metódy
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);

    // ← ZMENA: Jednoducho - len getter
    // TODO urob proper c++ a definiciu daj do .cpp tu nechaj len header
    bool getIsMovable() const { return isMovable; }
    void resetMouseState() { firstMouse = true; }
};

#endif