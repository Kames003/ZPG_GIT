#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <GLFW/glfw3.h>

class Camera;
class SceneManager;

class Controller {
private:
    // DVE KAMERY
    Camera* cameraStatic;
    Camera* cameraDynamic;
    Camera* cameraActive;   // Pointer na aktuálnu

    SceneManager* sceneManager;
    GLFWwindow* window;

    // Input state
    bool keysPressed[1024];

public:
    Controller(GLFWwindow* window, Camera* static_cam, Camera* dynamic_cam, SceneManager* sceneMgr);

    // Static GLFW callbacks
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode);

    // Update pohybu
    void updateMovement(float deltaTime);

    // Zmena aktívnej kamery
    void setActiveCamera(Camera* cam) { cameraActive = cam; }

};

#endif