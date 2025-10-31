#include "Controller.h"
#include "Camera.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "Light.h"
#include "DrawableObject.h"
#include "TransformComposite.h"
#include <cstdio>
#include <cstring>

Controller::Controller(GLFWwindow* window, Camera* static_cam, Camera* dynamic_cam,
                       SceneManager* sceneMgr, ShaderProgram* phong,
                       ShaderProgram* lambert, ShaderProgram* blinn,
                       ShaderProgram* phongWrong, ShaderProgram* phongCorrect)
    : cameraStatic(static_cam), cameraDynamic(dynamic_cam),
      sceneManager(sceneMgr), window(window),
      shaderPhong(phong), shaderLambert(lambert), shaderBlinn(blinn),
      shaderPhongWrong(phongWrong), shaderPhongCorrect(phongCorrect),
      usingCorrectShader(true)  // ← Predvolene SPRÁVNY shader
{
    memset(keysPressed, 0, sizeof(keysPressed));
    cameraActive = cameraStatic;

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

        // ========================================
        // Scene switching (1-7)
        // ========================================
        if (key >= GLFW_KEY_1 && key <= GLFW_KEY_7) {
            int sceneIndex = key - GLFW_KEY_1;
            ctrl->sceneManager->switchToScene(sceneIndex);

            Scene* scene = ctrl->sceneManager->getActiveScene();
            std::vector<Light*> lights;

            if (scene) {
                lights = scene->getLights();

                ctrl->shaderPhong->setLights(lights);
                ctrl->shaderLambert->setLights(lights);
                ctrl->shaderBlinn->setLights(lights);

                for (Light* light : lights) {
                    if (light) light->notifyAll();
                }
            }

            // Scéna 6 (les) je na indexe 5
            if (sceneIndex == 5) {
                ctrl->setActiveCamera(ctrl->cameraDynamic);
                ctrl->cameraDynamic->resetMouseState();
                ctrl->cameraDynamic->notifyAll();
                printf(">>> Scene 6 (Forest): Dynamic camera ENABLED <<<\n");
            }
            // Scéna 7 (backface test) je na indexe 6
            else if (sceneIndex == 6) {
                ctrl->cameraStatic->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
                ctrl->cameraStatic->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
                ctrl->setActiveCamera(ctrl->cameraStatic);
                ctrl->cameraStatic->notifyAll();

                ctrl->shaderPhongWrong->setLights(lights);
                ctrl->shaderPhongCorrect->setLights(lights);

                printf(">>> Scene 7 (Backface test): Press 'B' to toggle shaders <<<\n");
            }
            // Ostatné scény
            else {
                ctrl->cameraStatic->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
                ctrl->cameraStatic->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
                ctrl->setActiveCamera(ctrl->cameraStatic);
                ctrl->cameraStatic->notifyAll();
                printf(">>> Scene %d: Static camera <<<\n", sceneIndex + 1);
            }
            return;
        }

        // ========================================
        // BACKFACE SHADER TOGGLE (klávesa 'B')
        // ========================================
        if (key == GLFW_KEY_B) {
            // Len v scéne 7 (backface test)
            if (ctrl->sceneManager->getActiveSceneIndex() == 6) {
                Scene* scene = ctrl->sceneManager->getActiveScene();
                if (scene && scene->getObjects().size() > 0) {
                    DrawableObject* sphere = scene->getObjects()[0];

                    // Toggle medzi shadermi
                    ctrl->usingCorrectShader = !ctrl->usingCorrectShader;

                    if (ctrl->usingCorrectShader) {
                        sphere->setShaderProgram(ctrl->shaderPhongCorrect);
                        printf(">>> CORRECT shader (backface is DARK)<<<\n");
                    } else {
                        sphere->setShaderProgram(ctrl->shaderPhongWrong);
                        printf(">>> WRONG shader (backface GLOWS)<<<\n");
                    }

                    // Update svetiel pre nový shader
                    std::vector<Light*>& lights = scene->getLights();
                    ctrl->shaderPhongWrong->setLights(lights);
                    ctrl->shaderPhongCorrect->setLights(lights);

                    for (Light* light : lights) {
                        if (light) {
                            light->notifyAll();
                        }
                    }
                }
            }
            return;
        }

        // ========================================
        // Arrow keys - next scene
        // ========================================
        if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_N) {
            int currentIndex = ctrl->sceneManager->getActiveSceneIndex();
            int nextIndex = currentIndex + 1;

            if (nextIndex < ctrl->sceneManager->getSceneCount()) {
                ctrl->sceneManager->switchToNextScene();

                Scene* scene = ctrl->sceneManager->getActiveScene();
                if (scene) {
                    std::vector<Light*>& lights = scene->getLights();
                    ctrl->shaderPhong->setLights(lights);
                    ctrl->shaderLambert->setLights(lights);
                    ctrl->shaderBlinn->setLights(lights);
                    for (Light* light : lights) {
                        if (light) light->notifyAll();
                    }
                }

                // Scéna 6 je na indexe 5!
                if (nextIndex == 5) {
                    ctrl->setActiveCamera(ctrl->cameraDynamic);
                    ctrl->cameraDynamic->resetMouseState();
                    ctrl->cameraDynamic->notifyAll();
                    printf(">>> Scene 6: Dynamic camera ENABLED <<<\n");
                } else {
                    ctrl->setActiveCamera(ctrl->cameraStatic);
                    ctrl->cameraStatic->notifyAll();
                }
            }
            return;
        }

        // ========================================
        // Arrow keys - previous scene
        // ========================================
        if (key == GLFW_KEY_LEFT || key == GLFW_KEY_P) {
            int currentIndex = ctrl->sceneManager->getActiveSceneIndex();
            int prevIndex = currentIndex - 1;

            if (prevIndex >= 0) {
                ctrl->sceneManager->switchToPreviousScene();

                Scene* scene = ctrl->sceneManager->getActiveScene();
                if (scene) {
                    std::vector<Light*>& lights = scene->getLights();
                    ctrl->shaderPhong->setLights(lights);
                    ctrl->shaderLambert->setLights(lights);
                    ctrl->shaderBlinn->setLights(lights);
                    for (Light* light : lights) {
                        if (light) light->notifyAll();
                    }
                }

                // Scéna 6 je na indexe 5!
                if (prevIndex == 5) {
                    ctrl->setActiveCamera(ctrl->cameraDynamic);
                    ctrl->cameraDynamic->resetMouseState();
                    ctrl->cameraDynamic->notifyAll();
                    printf(">>> Scene 6: Dynamic camera ENABLED <<<\n");
                } else {
                    ctrl->setActiveCamera(ctrl->cameraStatic);
                    ctrl->cameraStatic->notifyAll();
                }
            }
            return;
        }

        if (key == GLFW_KEY_I) {
            // FOV = 45° (štandardné)
            Camera* activeCamera = (ctrl->sceneManager->getActiveSceneIndex() == 5)
                                    ? ctrl->cameraDynamic
                                    : ctrl->cameraStatic;

            activeCamera->setFOV(45.0f);
            activeCamera->flushPendingNotifications();
            printf("\n>>> FOV set to 45° (standard) <<<\n");
            return;
        }

        if (key == GLFW_KEY_O) {
            // FOV = 90° (wide)
            Camera* activeCamera = (ctrl->sceneManager->getActiveSceneIndex() == 5)
                                    ? ctrl->cameraDynamic
                                    : ctrl->cameraStatic;

            activeCamera->setFOV(90.0f);
            activeCamera->flushPendingNotifications();
            printf("\n>>> FOV set to 90° (wide angle) <<<\n");
            return;
        }

        if (key == GLFW_KEY_L) {
            // FOV = 130° (ultra-wide)
            Camera* activeCamera = (ctrl->sceneManager->getActiveSceneIndex() == 5)
                                    ? ctrl->cameraDynamic
                                    : ctrl->cameraStatic;

            activeCamera->setFOV(130.0f);
            activeCamera->flushPendingNotifications(); // chceme okamžitú zmenu --> user stlačil keyword chce vidieť zmenu hned
            printf("\n>>> FOV set to 130° (ultra-wide) <<<\n");
            return;
        }
    }
}

void Controller::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    Controller* ctrl = static_cast<Controller*>(glfwGetWindowUserPointer(window));
    if (!ctrl || !ctrl->cameraActive) return;

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

void Controller::updateMovement(float deltaTime)
{
    if (!cameraActive) return;

    if (keysPressed[GLFW_KEY_W])
        cameraActive->moveForward(deltaTime);
    if (keysPressed[GLFW_KEY_S])
        cameraActive->moveBackward(deltaTime);
    if (keysPressed[GLFW_KEY_A])
        cameraActive->moveLeft(deltaTime);
    if (keysPressed[GLFW_KEY_D])
        cameraActive->moveRight(deltaTime);
}