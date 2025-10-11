#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "SceneManager.h"
#include "Camera.h"

using namespace std;

// forward deklarácia
class Shader;
class ShaderProgram;
class Model;
class Transformation;
class DrawableObject;
class Scene;

class Application
{
private:
    GLFWwindow* window;
    SceneManager* sceneManager;
    Camera* camera;
    glm::mat4 staticViewMatrix; // statická view metrix pre scény 1-6

    // shaders
    Shader* vertexShader;
    Shader* fragmentShader1;
    Shader* fragmentShader2;

    // shader programs
    ShaderProgram* shaderProgram1;
    ShaderProgram* shaderProgram2;

    // models
    Model* model1;
    Model* model2;
    Model* model3;
    Model* modelSuziFlat;
    Model* modelSuziSmooth;
    Model* modelTree;
    Model* modelBushes;
    Model* modelGift;
    Model* modelPlane;
    Model* modelLamp;
    Model* modelBench;

    // Pre kameru a myš
    bool firstMouse;
    double lastX, lastY;
    double deltaTime;
    double lastFrame;



    // other variables
    double clickX, clickY;
    float rotationAngle;

    // NOVÉ! Pomocné metódy pre správu view matrix
    void restoreStaticViewMatrix();    // Obnoví statickú view matrix pre scény 1-6


public:
    Application();

    void initialization();
    void createShaders();
    void createModels();
    void createScenes();
    void run();

    void processInput(GLFWwindow* window);

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

    void error_callback(int error, const char* description);
    void window_focus_callback(GLFWwindow* window, int focused);
    void window_iconify_callback(GLFWwindow* window, int iconified);
    void window_size_callback(GLFWwindow* window, int width, int height);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mode);
};

#endif