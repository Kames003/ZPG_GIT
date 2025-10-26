#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <GL/glew.h>      // ← MUSÍ BYŤ PRVÝ!
#include <GLFW/glfw3.h>   // ← GLFW až potom

class Camera;
class SceneManager;
class ShaderProgram;

class Controller {
private:
    // DVE KAMERY
    Camera* cameraStatic;
    Camera* cameraDynamic;
    Camera* cameraActive;

    SceneManager* sceneManager;
    GLFWwindow* window;

    // SHADERY (pre update svetiel)
    ShaderProgram* shaderPhong;
    ShaderProgram* shaderLambert;
    ShaderProgram* shaderBlinn;
    ShaderProgram* shaderPhongWrong;
    ShaderProgram* shaderPhongCorrect;

    // Input state
    bool usingCorrectShader;
    bool keysPressed[1024];

public:
    Controller(GLFWwindow* window, Camera* static_cam, Camera* dynamic_cam,
               SceneManager* sceneMgr, ShaderProgram* phong,
               ShaderProgram* lambert, ShaderProgram* blinn,
               ShaderProgram* phongWrong, ShaderProgram* phongCorrect);


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