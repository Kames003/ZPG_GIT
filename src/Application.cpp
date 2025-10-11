#include "Application.h"

// 1. Systémové includes
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>

// 2. OpenGL/GLFW/GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// 3. GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 4. ZÁKLADNÉ TRIEDY (v správnom poradí dependency)
#include "Shader.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Camera.h"

// 5. TRANSFORMÁCIE (base class najprv!)
#include "Transformation.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "Composite.h"

// 6. OBJEKTY A SCÉNY
#include "DrawableObject.h"
#include "Scene.h"

// 7. MODEL DATA (raw arrays)
#include "sphere.h"
#include "suzi_flat.h"
#include "suzi_smooth.h"
#include "tree.h"
#include "bushes.h"
#include "gift.h"

Application::Application()
    : clickX(0), clickY(0), rotationAngle(0.0f),
      firstMouse(true), lastX(400.0), lastY(300.0),
      deltaTime(0.0), lastFrame(0.0)
{
    sceneManager = new SceneManager();
    camera = new Camera(glm::vec3(0.0f, 1.7f, 5.0f));

    // Statická view matrix pre scény 1-6 (2D pohľad)
    staticViewMatrix = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),   // pozícia kamery
        glm::vec3(0.0f, 0.0f, 0.0f),   // bod, na ktorý sa pozerá
        glm::vec3(0.0f, 1.0f, 0.0f)    // up vektor
    );
}

void Application::initialization()
{
    // GLFW init
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "ZPG project", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // GLEW init
    glewExperimental = GL_TRUE;
    glewInit();

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        app->key_callback(window, key, scancode, action, mods);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
    {
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        app->cursor_pos_callback(window, xpos, ypos);
    });

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
}

void Application::createShaders()
{
    const char* vertexShaderCode =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "layout(location=1) in vec3 vn;"
        "uniform mat4 modelMatrix;"
        "uniform mat4 viewMatrix;"
        "uniform mat4 projectionMatrix;"
        "void main () {"
        "     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);"
        "}";

    const char* fragmentShader1Code =
        "#version 330\n"
        "out vec4 fragColor;"
        "void main () {"
        "     fragColor = vec4(1.0, 0.0, 0.0, 1.0);"
        "}";

    const char* fragmentShader2Code =
        "#version 330\n"
        "out vec4 fragColor;"
        "void main () {"
        "     fragColor = vec4(0.0, 1.0, 0.0, 1.0);"
        "}";

    vertexShader = new Shader(vertexShaderCode, GL_VERTEX_SHADER);
    fragmentShader1 = new Shader(fragmentShader1Code, GL_FRAGMENT_SHADER);
    fragmentShader2 = new Shader(fragmentShader2Code, GL_FRAGMENT_SHADER);

    std::vector<Shader*> shaders1 = {vertexShader, fragmentShader1};
    shaderProgram1 = new ShaderProgram(shaders1);

    std::vector<Shader*> shaders2 = {vertexShader, fragmentShader2};
    shaderProgram2 = new ShaderProgram(shaders2);

    // Registrácia kamery ako observer
    camera->attach(shaderProgram1);
    camera->attach(shaderProgram2);

    // Nastavenie projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    shaderProgram1->use();
    shaderProgram1->setUniform("projectionMatrix", projection);
    shaderProgram2->use();
    shaderProgram2->setUniform("projectionMatrix", projection);

    // Nastavenie POČIATOČNEJ view matrix (statická pre scény 1-6)
    shaderProgram1->use();
    shaderProgram1->setUniform("viewMatrix", staticViewMatrix);
    shaderProgram2->use();
    shaderProgram2->setUniform("viewMatrix", staticViewMatrix);

    printf("Shaders created successfully!\n");
    printf("Observer pattern: ShaderProgram registered to Camera\n");
}

void Application::createModels()
{
    float triangleVertices[] =
    {
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    float squareVertices[] =
    {
        -0.3f, -0.3f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.3f, -0.3f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.3f,  0.3f, 0.0f,  0.0f, 0.0f, 1.0f,
        -0.3f, -0.3f, 0.0f,  0.0f, 0.0f, 1.0f,
         0.3f,  0.3f, 0.0f,  0.0f, 0.0f, 1.0f,
        -0.3f,  0.3f, 0.0f,  0.0f, 0.0f, 1.0f
    };

    model1 = new Model(triangleVertices, 3);
    model2 = new Model(squareVertices, 6);
    model3 = new Model((float*)sphere, 2880);
    modelSuziFlat = new Model((float*)suziFlat, 2904);
    modelSuziSmooth = new Model((float*)suziSmooth, 2904);
    modelTree = new Model((float*)tree, 92814);
    modelBushes = new Model((float*)bushes, 8730);
    modelGift = new Model((float*)gift, 66624);

    printf("Models created successfully!\n");
}

void Application::createScenes()
{
    // SCENE 1: red triangle on the left
    Scene* scene1 = new Scene();
    Translate* t1 = new Translate(-0.5f, 0.0f, 0.0f);
    DrawableObject* obj1 = new DrawableObject(model1, t1, shaderProgram1);
    scene1->addObject(obj1);
    sceneManager->addScene(scene1);

    // SCENE 2: green square on the right with composite transformation
    Scene* scene2 = new Scene();
    Composite* composite = new Composite();
    composite->addTransformation(new Translate(0.5f, 0.0f, 0.0f));
    composite->addTransformation(new Scale(0.7f, 0.7f, 1.0f));
    DrawableObject* obj2 = new DrawableObject(model2, composite, shaderProgram2);
    scene2->addObject(obj2);
    sceneManager->addScene(scene2);

    // SCENE 3: both objects at the same time
    Scene* scene3 = new Scene();
    Translate* t4 = new Translate(-0.5f, 0.0f, 0.0f);
    DrawableObject* obj3 = new DrawableObject(model1, t4, shaderProgram1);
    Translate* t5 = new Translate(0.5f, 0.0f, 0.0f);
    DrawableObject* obj4 = new DrawableObject(model2, t5, shaderProgram2);
    scene3->addObject(obj3);
    scene3->addObject(obj4);
    sceneManager->addScene(scene3);

    // SCENE 4: rotating triangle
    Scene* scene4 = new Scene();
    Composite* rotatingComposite = new Composite();
    rotatingComposite->addTransformation(new Rotate(0.0f, 0.0f, 0.0f, 1.0f));
    DrawableObject* obj5 = new DrawableObject(model1, rotatingComposite, shaderProgram1);
    scene4->addObject(obj5);
    sceneManager->addScene(scene4);

    // SCENE 5: four balls on the axes
    Scene* scene5 = new Scene();

    // ball on the right
    Composite* comp1 = new Composite();
    comp1->addTransformation(new Translate(0.5f, 0.0f, 0.0f));
    comp1->addTransformation(new Scale(0.1f, 0.1f, 0.1f));
    DrawableObject* sphere1 = new DrawableObject(model3, comp1, shaderProgram1);
    scene5->addObject(sphere1);

    // ball on the left
    Composite* comp2 = new Composite();
    comp2->addTransformation(new Translate(-0.5f, 0.0f, 0.0f));
    comp2->addTransformation(new Scale(0.1f, 0.1f, 0.1f));
    DrawableObject* sphere2 = new DrawableObject(model3, comp2, shaderProgram2);
    scene5->addObject(sphere2);

    // ball on the top
    Composite* comp3 = new Composite();
    comp3->addTransformation(new Translate(0.0f, 0.5f, 0.0f));
    comp3->addTransformation(new Scale(0.1f, 0.1f, 0.1f));
    DrawableObject* sphere3 = new DrawableObject(model3, comp3, shaderProgram1);
    scene5->addObject(sphere3);

    // ball on the bottom
    Composite* comp4 = new Composite();
    comp4->addTransformation(new Translate(0.0f, -0.5f, 0.0f));
    comp4->addTransformation(new Scale(0.1f, 0.1f, 0.1f));
    DrawableObject* sphere4 = new DrawableObject(model3, comp4, shaderProgram2);
    scene5->addObject(sphere4);

    sceneManager->addScene(scene5);

    // SCENE 6: complex scene with 20+ objects
    Scene* scene6 = new Scene();

    // 4 suzi flat (red) - in the corners
    Composite* suzi1 = new Composite();
    suzi1->addTransformation(new Translate(-0.7f, 0.7f, 0.0f));
    suzi1->addTransformation(new Scale(0.12f, 0.12f, 0.12f));
    scene6->addObject(new DrawableObject(modelSuziFlat, suzi1, shaderProgram1));

    Composite* suzi2 = new Composite();
    suzi2->addTransformation(new Translate(0.7f, 0.7f, 0.0f));
    suzi2->addTransformation(new Scale(0.12f, 0.12f, 0.12f));
    scene6->addObject(new DrawableObject(modelSuziFlat, suzi2, shaderProgram1));

    Composite* suzi3 = new Composite();
    suzi3->addTransformation(new Translate(-0.7f, -0.7f, 0.0f));
    suzi3->addTransformation(new Scale(0.12f, 0.12f, 0.12f));
    scene6->addObject(new DrawableObject(modelSuziFlat, suzi3, shaderProgram1));

    Composite* suzi4 = new Composite();
    suzi4->addTransformation(new Translate(0.7f, -0.7f, 0.0f));
    suzi4->addTransformation(new Scale(0.12f, 0.12f, 0.12f));
    scene6->addObject(new DrawableObject(modelSuziFlat, suzi4, shaderProgram1));

    // 5 trees (green) - top row
    Composite* tr1 = new Composite();
    tr1->addTransformation(new Translate(-0.6f, 0.35f, 0.0f));
    tr1->addTransformation(new Scale(0.03f, 0.03f, 0.03f));
    scene6->addObject(new DrawableObject(modelTree, tr1, shaderProgram2));

    Composite* tr2 = new Composite();
    tr2->addTransformation(new Translate(-0.3f, 0.35f, 0.0f));
    tr2->addTransformation(new Scale(0.03f, 0.03f, 0.03f));
    scene6->addObject(new DrawableObject(modelTree, tr2, shaderProgram2));

    Composite* tr3 = new Composite();
    tr3->addTransformation(new Translate(0.0f, 0.35f, 0.0f));
    tr3->addTransformation(new Scale(0.03f, 0.03f, 0.03f));
    scene6->addObject(new DrawableObject(modelTree, tr3, shaderProgram2));

    Composite* tr4 = new Composite();
    tr4->addTransformation(new Translate(0.3f, 0.35f, 0.0f));
    tr4->addTransformation(new Scale(0.03f, 0.03f, 0.03f));
    scene6->addObject(new DrawableObject(modelTree, tr4, shaderProgram2));

    Composite* tr5 = new Composite();
    tr5->addTransformation(new Translate(0.6f, 0.35f, 0.0f));
    tr5->addTransformation(new Scale(0.03f, 0.03f, 0.03f));
    scene6->addObject(new DrawableObject(modelTree, tr5, shaderProgram2));

    // 5 bushes (red) - bottom row
    Composite* bu1 = new Composite();
    bu1->addTransformation(new Translate(-0.5f, -0.35f, 0.0f));
    bu1->addTransformation(new Scale(0.15f, 0.15f, 0.15f));
    scene6->addObject(new DrawableObject(modelBushes, bu1, shaderProgram1));

    Composite* bu2 = new Composite();
    bu2->addTransformation(new Translate(-0.25f, -0.35f, 0.0f));
    bu2->addTransformation(new Scale(0.15f, 0.15f, 0.15f));
    scene6->addObject(new DrawableObject(modelBushes, bu2, shaderProgram1));

    Composite* bu3 = new Composite();
    bu3->addTransformation(new Translate(0.0f, -0.35f, 0.0f));
    bu3->addTransformation(new Scale(0.15f, 0.15f, 0.15f));
    scene6->addObject(new DrawableObject(modelBushes, bu3, shaderProgram1));

    Composite* bu4 = new Composite();
    bu4->addTransformation(new Translate(0.25f, -0.35f, 0.0f));
    bu4->addTransformation(new Scale(0.15f, 0.15f, 0.15f));
    scene6->addObject(new DrawableObject(modelBushes, bu4, shaderProgram1));

    Composite* bu5 = new Composite();
    bu5->addTransformation(new Translate(0.5f, -0.35f, 0.0f));
    bu5->addTransformation(new Scale(0.15f, 0.15f, 0.15f));
    scene6->addObject(new DrawableObject(modelBushes, bu5, shaderProgram1));

    // 4 balls (green) - on the edges
    Composite* sph1 = new Composite();
    sph1->addTransformation(new Translate(0.9f, 0.0f, 0.0f));
    sph1->addTransformation(new Scale(0.07f, 0.07f, 0.07f));
    scene6->addObject(new DrawableObject(model3, sph1, shaderProgram2));

    Composite* sph2 = new Composite();
    sph2->addTransformation(new Translate(-0.9f, 0.0f, 0.0f));
    sph2->addTransformation(new Scale(0.07f, 0.07f, 0.07f));
    scene6->addObject(new DrawableObject(model3, sph2, shaderProgram2));

    Composite* sph3 = new Composite();
    sph3->addTransformation(new Translate(0.0f, 0.9f, 0.0f));
    sph3->addTransformation(new Scale(0.07f, 0.07f, 0.07f));
    scene6->addObject(new DrawableObject(model3, sph3, shaderProgram2));

    Composite* sph4 = new Composite();
    sph4->addTransformation(new Translate(0.0f, -0.9f, 0.0f));
    sph4->addTransformation(new Scale(0.07f, 0.07f, 0.07f));
    scene6->addObject(new DrawableObject(model3, sph4, shaderProgram2));

    // 4 gifts (red) - in the middle
    Composite* gi1 = new Composite();
    gi1->addTransformation(new Translate(-0.15f, 0.1f, 0.0f));
    gi1->addTransformation(new Scale(0.18f, 0.18f, 0.18f));
    scene6->addObject(new DrawableObject(modelGift, gi1, shaderProgram1));

    Composite* gi2 = new Composite();
    gi2->addTransformation(new Translate(0.15f, 0.1f, 0.0f));
    gi2->addTransformation(new Scale(0.18f, 0.18f, 0.18f));
    scene6->addObject(new DrawableObject(modelGift, gi2, shaderProgram1));

    Composite* gi3 = new Composite();
    gi3->addTransformation(new Translate(-0.15f, -0.1f, 0.0f));
    gi3->addTransformation(new Scale(0.18f, 0.18f, 0.18f));
    scene6->addObject(new DrawableObject(modelGift, gi3, shaderProgram1));

    Composite* gi4 = new Composite();
    gi4->addTransformation(new Translate(0.15f, -0.1f, 0.0f));
    gi4->addTransformation(new Scale(0.18f, 0.18f, 0.18f));
    scene6->addObject(new DrawableObject(modelGift, gi4, shaderProgram1));

    sceneManager->addScene(scene6);

    // SCENE 7: Procedurálne generovaný les (50-80 stromov a kríkov)
    Scene* forestScene = new Scene();

    srand(time(0));

    int treeCount = 50 + rand() % 31;  // 50-80 stromov

    printf("Generating forest scene 7 with %d trees...\n", treeCount);

    for (int i = 0; i < treeCount; i++)
    {
        // Náhodná pozícia v rozsahu -10 až 10 (x a z)
        float x = -10.0f + static_cast<float>(rand()) / RAND_MAX * 20.0f;
        float z = -10.0f + static_cast<float>(rand()) / RAND_MAX * 20.0f;

        // Náhodná škála 0.02 - 0.05
        float scale = 0.02f + static_cast<float>(rand()) / RAND_MAX * 0.03f;

        // Strom (červený kmeň)
        Composite* treeTransform = new Composite();
        treeTransform->addTransformation(new Translate(x, 0.0f, z));
        treeTransform->addTransformation(new Scale(scale, scale, scale));
        forestScene->addObject(new DrawableObject(modelTree, treeTransform, shaderProgram1));

        // Lístie/bush (zelený) - trochu nad zemou
        Composite* bushTransform = new Composite();
        bushTransform->addTransformation(new Translate(x, 0.3f, z));
        bushTransform->addTransformation(new Scale(scale * 2.0f, scale * 2.0f, scale * 2.0f));
        forestScene->addObject(new DrawableObject(modelBushes, bushTransform, shaderProgram2));
    }

    sceneManager->addScene(forestScene);

    printf("Scenes created successfully!\n");
    printf("Scene 1: Red triangle\n");
    printf("Scene 2: Green square\n");
    printf("Scene 3: Both objects\n");
    printf("Scene 4: Rotating triangle\n");
    printf("Scene 5: 4 spheres on axes\n");
    printf("Scene 6: Complex scene with 22 objects\n");
    printf("Scene 7: Procedural forest (camera controls: WSAD + mouse)\n");
    printf("Press keys 1-7 to switch scenes\n");
}

void Application::run()
{
    while (!glfwWindowShouldClose(window))
    {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input processing pre scénu 7
        if (sceneManager->getActiveSceneIndex() == 6)  // Scéna 7 (index 6)
        {
            processInput(window);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rotation update for scene 4
        if (sceneManager->getActiveSceneIndex() == 3)  // scene 4 (index 3)
        {
            rotationAngle += 1.0f;
            if (rotationAngle > 360.0f) rotationAngle -= 360.0f;

            Scene* scene4 = sceneManager->getActiveScene();
            if (scene4)
            {
                DrawableObject* obj = scene4->getObjects()[0];
                Composite* comp = dynamic_cast<Composite*>(obj->getTransformation());
                if (comp)
                {
                    comp->clear();
                    comp->addTransformation(new Rotate(rotationAngle, 0.0f, 0.0f, 1.0f));
                }
            }
        }

        // Draw active scene
        sceneManager->drawActiveScene();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->moveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->moveRight(deltaTime);
}

void Application::restoreStaticViewMatrix()
{
    // Obnoví statickú view matrix pre scény 1-6
    shaderProgram1->use();
    shaderProgram1->setUniform("viewMatrix", staticViewMatrix);
    shaderProgram2->use();
    shaderProgram2->setUniform("viewMatrix", staticViewMatrix);

    printf("View matrix restored to static (2D scenes)\n");
}

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
    {
        // Prepínanie scén klávesami 1-7
        if (key >= GLFW_KEY_1 && key <= GLFW_KEY_7)
        {
            int sceneIndex = key - GLFW_KEY_1;  // 0-6
            sceneManager->switchToScene(sceneIndex);

            // ========== SUPER JEDNODUCHÁ LOGIKA ==========
            if (sceneIndex == 6)  // Prepnutie NA scénu 7
            {
                firstMouse = true;
                shaderProgram1->use();
                shaderProgram1->setUniform("viewMatrix", camera->getViewMatrix());
                shaderProgram2->use();
                shaderProgram2->setUniform("viewMatrix", camera->getViewMatrix());
            }
            else  // Prepnutie NA scény 1-6
            {
                restoreStaticViewMatrix();  // Obnov statickú view matrix
            }
            // =============================================
        }
        // BONUS: Šípky pre next/previous
        else if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_N)
        {
            sceneManager->switchToNextScene();
            int newSceneIndex = sceneManager->getActiveSceneIndex();

            if (newSceneIndex == 6)
            {
                firstMouse = true;
                shaderProgram1->use();
                shaderProgram1->setUniform("viewMatrix", camera->getViewMatrix());
                shaderProgram2->use();
                shaderProgram2->setUniform("viewMatrix", camera->getViewMatrix());
            }
            else
            {
                restoreStaticViewMatrix();
            }
        }
        else if (key == GLFW_KEY_LEFT || key == GLFW_KEY_P)
        {
            sceneManager->switchToPreviousScene();
            int newSceneIndex = sceneManager->getActiveSceneIndex();

            if (newSceneIndex == 6)
            {
                firstMouse = true;
                shaderProgram1->use();
                shaderProgram1->setUniform("viewMatrix", camera->getViewMatrix());
                shaderProgram2->use();
                shaderProgram2->setUniform("viewMatrix", camera->getViewMatrix());
            }
            else
            {
                restoreStaticViewMatrix();
            }
        }
    }
}

void Application::error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

void Application::window_focus_callback(GLFWwindow* window, int focused)
{
    printf("window_focus_callback \n");
}

void Application::window_iconify_callback(GLFWwindow* window, int iconified)
{
    printf("window_iconify_callback \n");
}

void Application::window_size_callback(GLFWwindow* window, int width, int height)
{
    printf("resize %d, %d \n", width, height);
    glViewport(0, 0, width, height);
}

void Application::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Ak NIE sme v scéne 7, použi starú funkcionalitu
    if (sceneManager->getActiveSceneIndex() != 6)
    {
        printf("cursor_pos_callback %d, %d; %d, %d\n", (int)xpos, (int)ypos, (int)clickX, (int)clickY);
        return;
    }

    // Obsluha kamery pre scénu 7
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xOffset = xpos - lastX;
    double yOffset = lastY - ypos;  // Reversed: y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->processMouseMovement(xOffset, yOffset);
}

void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
    if (action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &clickX, &clickY);
        printf("mouse_button_callback [%d,%d,%d]\n", button, action, mode);
    }
}