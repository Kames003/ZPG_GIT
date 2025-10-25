#include "Contoller.h"
#include "Camera.h"
#include "SceneManager.h"
#include <cstdio>
#include <cstring>

Controller::Controller(GLFWwindow* window, Camera* static_cam, Camera* dynamic_cam, SceneManager* sceneMgr)
    : cameraStatic(static_cam), cameraDynamic(dynamic_cam),
      sceneManager(sceneMgr), window(window)
{
    memset(keysPressed, 0, sizeof(keysPressed));
    cameraActive = cameraStatic;  // Štart so static

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, Controller::keyCallback);
    glfwSetCursorPosCallback(window, Controller::cursorCallback);
    glfwSetMouseButtonCallback(window, Controller::mouseButtonCallback);
}

void Controller::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Controller* ctrl = static_cast<Controller*>(glfwGetWindowUserPointer(window));
    if (!ctrl) return;

    // Tracking klávesnice
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            ctrl->keysPressed[key] = true;
        else if (action == GLFW_RELEASE)
            ctrl->keysPressed[key] = false;
    }

    if (action == GLFW_PRESS) {
        // ESC
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GL_TRUE);
            return;
        }

        // Scene switching (1-7)
        if (key >= GLFW_KEY_1 && key <= GLFW_KEY_7) {
            int sceneIndex = key - GLFW_KEY_1;
            ctrl->sceneManager->switchToScene(sceneIndex);

            // ← ZMENA: Jasná logika prepínania kamery + notifyObservers
            if (sceneIndex == 6) {  // Scéna 7 = les
                ctrl->setActiveCamera(ctrl->cameraDynamic);
                ctrl->cameraDynamic->resetMouseState();
                ctrl->cameraDynamic->notifyAll();
                printf(">>> Scene 7: Dynamic camera enabled <<<\n");
            } else {  // Scény 1-6
                ctrl->setActiveCamera(ctrl->cameraStatic);
                ctrl->cameraStatic->notifyAll();
                printf(">>> Scene %d: Static camera <<<\n", sceneIndex + 1);
            }
            return;
        }

        // Arrow keys - next scene
        if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_N) {
            int currentIndex = ctrl->sceneManager->getActiveSceneIndex();
            int nextIndex = currentIndex + 1;

            if (nextIndex < ctrl->sceneManager->getSceneCount()) {
                ctrl->sceneManager->switchToNextScene();

                if (nextIndex == 6) {
                    ctrl->setActiveCamera(ctrl->cameraDynamic);
                    ctrl->cameraDynamic->notifyAll();  // ← PRIDAJTE
                } else {
                    ctrl->setActiveCamera(ctrl->cameraStatic);
                    ctrl->cameraStatic->notifyAll();  // ← PRIDAJTE
                }
            }
            return;
        }

        // Arrow keys - previous scene
        if (key == GLFW_KEY_LEFT || key == GLFW_KEY_P) {
            int currentIndex = ctrl->sceneManager->getActiveSceneIndex();
            int prevIndex = currentIndex - 1;

            if (prevIndex >= 0) {
                ctrl->sceneManager->switchToPreviousScene();

                if (prevIndex == 6) {
                    ctrl->setActiveCamera(ctrl->cameraDynamic);
                    ctrl->cameraDynamic->notifyAll();  // ← PRIDAJTE
                } else {
                    ctrl->setActiveCamera(ctrl->cameraStatic);
                    ctrl->cameraStatic->notifyAll();  // ← PRIDAJTE
                }
            }
            return;
        }
    }
}

void Controller::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    Controller* ctrl = static_cast<Controller*>(glfwGetWindowUserPointer(window));
    if (!ctrl || !ctrl->cameraActive) return;

    // ← ZMENA: Delegujeme na aktuálnu kameru
    ctrl->cameraActive->processMouseInput(xpos, ypos);
}

void Controller::mouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
{
    if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        printf("Mouse button [%d] clicked at [%.0f, %.0f]\n", button, xpos, ypos);
    }
}

// ← NOVÁ metóda
void Controller::updateMovement(float deltaTime)
{
    if (!cameraActive) return;

    // Všetky klávesy naraz
    if (keysPressed[GLFW_KEY_W])
        cameraActive->moveForward(deltaTime);
    if (keysPressed[GLFW_KEY_S])
        cameraActive->moveBackward(deltaTime);
    if (keysPressed[GLFW_KEY_A])
        cameraActive->moveLeft(deltaTime);
    if (keysPressed[GLFW_KEY_D])
        cameraActive->moveRight(deltaTime);
}