#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "SceneManager.h"
#include "Camera.h"
#include "Contoller.h"
#include "Light.h"

using namespace std;

class GLFWwindow;
class Shader;
class ShaderProgram;
class Model;

class Application
{
private:
    GLFWwindow* window;
    SceneManager* sceneManager;

    Camera* cameraStatic;    // Scény 1-6
    Camera* cameraDynamic;   // Scéna 7 (les)

    Controller* controller;

    // Shaders
    Shader* vertexShader;
    Shader* fragmentShader1;
    Shader* fragmentShader2;
    ShaderProgram* shaderProgram1;
    ShaderProgram* shaderProgram2;
    ShaderProgram* shaderProgramTree;
    ShaderProgram* shaderProgramBush;
    ShaderProgram* shaderProgramGround;
    ShaderProgram* shaderProgramPath;
    ShaderProgram* shaderProgramBench;

    // lightning shaders


    ShaderProgram* shaderProgramPhong;
    ShaderProgram* shaderProgramLambert;
    ShaderProgram* shaderProgramConstant;
    ShaderProgram* shaderProgramBlinn;

    Light* mainLight;


    // Models
    Model* model1;
    Model* model2;
    Model* model3;
    Model* modelSuziFlat;
    Model* modelSuziSmooth;
    Model* modelTree;
    Model* modelBushes;
    Model* modelGift;
    Model* modelPlain;
    Model* modelBench;

    float rotationAngle;

public:
    Application();
    ~Application();

    void initialization();
    void createShaders();
    void createModels();
    void createScenes();
    void run();

    // ← NOVÉ: Gettery pre controller
    Camera* getCameraStatic() { return cameraStatic; }
    Camera* getCameraDynamic() { return cameraDynamic; }
};

#endif