#include "Application.h"

// 1. Systémové includes
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>

// 2. OpenGL/GLFW/GLEW
#include <GLFW/glfw3.h>
#include <GL/glew.h>

// 3. GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 4. ZÁKLADNÉ TRIEDY
#include "Shader.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Camera.h"
#include "Contoller.h"
#include "ShaderLoader.h"


// 5. TRANSFORMÁCIE
#include "TransformComponent.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "TransformComposite.h"

// 6. OBJEKTY A SCÉNY
#include "DrawableObject.h"
#include "Scene.h"
#include "SceneFactory.h"


// 7. MODEL DATA
#include "models/sphere.h"
#include "models/suzi_flat.h"
#include "models/suzi_smooth.h"
#include "models/tree.h"
#include "models/bushes.h"
#include "models/gift.h"
#include "models/plain.h"
#include "models/bench.h"
#include "models/triangle.h"
#include "models/square.h"


Application::Application()
    : rotationAngle(0.0f)
{
    sceneManager = new SceneManager();

    cameraStatic = new Camera(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f,
        0.0f,
        false
    );

    cameraDynamic = new Camera(
        glm::vec3(0.0f, 0.3f, 5.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f,
        0.0f,
        true
    );
}

Application::~Application()
{
    delete cameraStatic;
    delete cameraDynamic;
    delete controller;
    delete sceneManager;
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

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    printf("Window size: %dx%d\n", width, height);

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

    controller = new Controller(window, cameraStatic, cameraDynamic, sceneManager);

    glEnable(GL_DEPTH_TEST);
}

void Application::createShaders()
{
    printf("\n========================================\n");
    printf("CREATING SHADERS FROM FILES\n");
    printf("========================================\n");

    // ========================================
    // STATIC CAMERA SHADERY (scény 1-6) - ZO SÚBOROV
    // ========================================

    printf("\n--- Creating basic shaders from files ---\n");

    vertexShader = new Shader(GL_VERTEX_SHADER,
        new FileShaderLoader("../src/shaders/simple_vertex.vert"));

    fragmentShader1 = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/red_fragment.frag"));

    fragmentShader2 = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/green_fragment.frag"));

    std::vector<Shader*> shaders1 = {vertexShader, fragmentShader1};
    shaderProgram1 = new ShaderProgram(shaders1);

    std::vector<Shader*> shaders2 = {vertexShader, fragmentShader2};
    shaderProgram2 = new ShaderProgram(shaders2);

    // Pripoj statickú kameru
    cameraStatic->attach(shaderProgram1);
    cameraStatic->attach(shaderProgram2);

    // Projection matrix - DYNAMICKÝ ASPECT RATIO
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspectRatio = (float)width / (float)height;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    printf("Aspect ratio: %.3f (%dx%d)\n", aspectRatio, width, height);

    shaderProgram1->use();
    shaderProgram1->setUniform("projectionMatrix", projection);

    shaderProgram2->use();
    shaderProgram2->setUniform("projectionMatrix", projection);

    cameraStatic->notifyAll();

    printf("✓ Static camera shaders created from files\n");

    // ========================================
    // DYNAMIC CAMERA SHADERY (scéna 7 - les) - ZO SÚBOROV
    // ========================================

    printf("\n--- Creating forest shaders from files ---\n");

    Shader* forestVertex = new Shader(GL_VERTEX_SHADER,
        new FileShaderLoader("../src/shaders/forest_vertex.vert"));

    Shader* treeFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/tree_fragment.frag"));

    Shader* bushFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/bush_fragment.frag"));

    Shader* groundFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/ground_fragment.frag"));

    Shader* pathFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/path_fragment.frag"));

    Shader* benchFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/bench_fragment.frag"));

    std::vector<Shader*> treeShaders = {forestVertex, treeFragment};
    shaderProgramTree = new ShaderProgram(treeShaders);

    std::vector<Shader*> bushShaders = {forestVertex, bushFragment};
    shaderProgramBush = new ShaderProgram(bushShaders);

    std::vector<Shader*> groundShaders = {forestVertex, groundFragment};
    shaderProgramGround = new ShaderProgram(groundShaders);

    std::vector<Shader*> pathShaders = {forestVertex, pathFragment};
    shaderProgramPath = new ShaderProgram(pathShaders);

    std::vector<Shader*> benchShaders = {forestVertex, benchFragment};
    shaderProgramBench = new ShaderProgram(benchShaders);

    // Pripoj dynamickú kameru
    cameraDynamic->attach(shaderProgramTree);
    cameraDynamic->attach(shaderProgramBush);
    cameraDynamic->attach(shaderProgramGround);
    cameraDynamic->attach(shaderProgramPath);
    cameraDynamic->attach(shaderProgramBench);

    // Projection matrix pre lesné shadery
    shaderProgramTree->use();
    shaderProgramTree->setUniform("projectionMatrix", projection);

    shaderProgramBush->use();
    shaderProgramBush->setUniform("projectionMatrix", projection);

    shaderProgramGround->use();
    shaderProgramGround->setUniform("projectionMatrix", projection);

    shaderProgramPath->use();
    shaderProgramPath->setUniform("projectionMatrix", projection);

    shaderProgramBench->use();
    shaderProgramBench->setUniform("projectionMatrix", projection);

    cameraDynamic->notifyAll();

    printf("✓ Dynamic camera shaders created from files\n");

    // ========================================
    // LIGHTING SHADERY (zo súborov)
    // ========================================

    printf("\n--- Creating lighting shaders from files ---\n");

    Shader* lightingVertex = new Shader(GL_VERTEX_SHADER,
        new FileShaderLoader("../src/shaders/phong_vertex.vert"));

    Shader* phongFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/phong_fragment.frag"));

    Shader* lambertFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/lambert_fragment.frag"));

    Shader* constantFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/constant_fragment.frag"));

    Shader* blinnFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/blinn_fragment.frag"));

    // Vytvor shader programy
    std::vector<Shader*> phongShaders = {lightingVertex, phongFragment};
    shaderProgramPhong = new ShaderProgram(phongShaders);

    std::vector<Shader*> lambertShaders = {lightingVertex, lambertFragment};
    shaderProgramLambert = new ShaderProgram(lambertShaders);

    std::vector<Shader*> constantShaders = {lightingVertex, constantFragment};
    shaderProgramConstant = new ShaderProgram(constantShaders);

    std::vector<Shader*> blinnShaders = {lightingVertex, blinnFragment};
    shaderProgramBlinn = new ShaderProgram(blinnShaders);

    // ========================================
    // LIGHT SETUP
    // ========================================

    printf("\n--- Setting up light ---\n");

    mainLight = new Light(
        glm::vec3(0.0f, 0.0f, 0.2f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f
    );

    printf("Light position: (%.1f, %.1f, %.1f)\n",
           mainLight->getPosition().x,
           mainLight->getPosition().y,
           mainLight->getPosition().z);

    // Pripoj light observers
    mainLight->attach(shaderProgramPhong);
    mainLight->attach(shaderProgramLambert);
    mainLight->attach(shaderProgramBlinn);

    // Pripoj kamery k lighting shaderom
    cameraStatic->attach(shaderProgramPhong);
    cameraStatic->attach(shaderProgramLambert);
    cameraStatic->attach(shaderProgramConstant);
    cameraStatic->attach(shaderProgramBlinn);

    cameraDynamic->attach(shaderProgramPhong);
    cameraDynamic->attach(shaderProgramLambert);
    cameraDynamic->attach(shaderProgramConstant);
    cameraDynamic->attach(shaderProgramBlinn);

    // Projection matrix pre lighting shadery
    shaderProgramPhong->use();
    shaderProgramPhong->setUniform("projectionMatrix", projection);

    shaderProgramLambert->use();
    shaderProgramLambert->setUniform("projectionMatrix", projection);

    shaderProgramConstant->use();
    shaderProgramConstant->setUniform("projectionMatrix", projection);

    shaderProgramBlinn->use();
    shaderProgramBlinn->setUniform("projectionMatrix", projection);

    // KRITICKÉ: Najprv notifikuj svetlo, potom kameru!
    printf("\n--- Sending initial notifications ---\n");
    mainLight->notifyAll();
    printf("✓ Light notifications sent\n");

    cameraStatic->notifyAll();
    printf("✓ Camera notifications sent\n");

    printf("\n✓ Lighting shaders created: Phong, Lambert, Constant, Blinn\n");

    printf("\n========================================\n");
    printf("✓ ALL SHADERS CREATED FROM FILES!\n");
    printf("✓ Static camera → simple shaders\n");
    printf("✓ Dynamic camera → forest shaders\n");
    printf("✓ Lighting → Phong, Lambert, Constant, Blinn\n");
    printf("========================================\n\n");
}


void Application::createModels()
{

    modelBench = new Model(benchVertices, 108);
    model1 = new Model((float*)triangle, 3);
    model2 = new Model((float*)square, 6);
    model3 = new Model((float*)sphere, 2880);
    modelSuziFlat = new Model((float*)suziFlat, 2904);
    modelSuziSmooth = new Model((float*)suziSmooth, 2904);
    modelTree = new Model((float*)tree, 92814);
    modelBushes = new Model((float*)bushes, 8730);
    modelGift = new Model((float*)gift, 66624);
    modelPlain = new Model((float*)plain, 6);

    printf("Models created successfully!\n");
}

void Application::createScenes()
{
    printf("\n========================================\n");
    printf("CREATING SCENES VIA FACTORY\n");
    printf("========================================\n");

    // ========================================
    // VYTVOR FACTORY
    // ========================================
    SceneFactory* factory = new SceneFactory(
        // Modely
        model1, model2, model3, modelSuziFlat, modelSuziSmooth,
        modelTree, modelBushes, modelGift, modelPlain, modelBench,

        // Základné shadery
        shaderProgram1, shaderProgram2,

        // Lighting shadery
        shaderProgramPhong, shaderProgramLambert,
        shaderProgramConstant, shaderProgramBlinn,

        // Lesné shadery
        shaderProgramTree, shaderProgramBush,
        shaderProgramGround, shaderProgramPath, shaderProgramBench
    );

    // ========================================
    // VYTVOR VŠETKY SCÉNY
    // ========================================
    sceneManager->addScene(factory->createPhongTestScene());          // Scéna 1
    sceneManager->addScene(factory->createSolarSystemScene());        // Scéna 2
    sceneManager->addScene(factory->createBothObjectsScene());        // Scéna 3
    sceneManager->addScene(factory->createRotatingTriangleScene());   // Scéna 4
    sceneManager->addScene(factory->createLightingDemoScene());       // Scéna 5
    sceneManager->addScene(factory->createComplexScene());            // Scéna 6
    sceneManager->addScene(factory->createForestScene());             // Scéna 7

    // ========================================
    // ZMAŽ FACTORY
    // ========================================
    delete factory;

    printf("========================================\n");
    printf("ALL SCENES CREATED!\n");
    printf("Total scenes: %d\n", sceneManager->getSceneCount());
    printf("Press keys 1-7 to switch scenes\n");
    printf("========================================\n\n");
}

void Application::run()
{
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        if (sceneManager->getActiveSceneIndex() == 6)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        controller->updateMovement(deltaTime);

        // ============================================
        // SCENE 2: SOLAR SYSTEM ANIMATION
        // ============================================
        if (sceneManager->getActiveSceneIndex() == 1) {
            // ✅ Priamo z času - žiadne static premenné!
            float currentTime = (float)glfwGetTime();
            float earthAngle = fmod(currentTime * 30.0f, 360.0f);  // 30°/s
            float moonAngle = fmod(currentTime * 48.0f, 360.0f);   // 48°/s

            Scene* solarScene = sceneManager->getActiveScene();
            if (solarScene && solarScene->getObjects().size() >= 3) {

                // Earth (index 1)
                DrawableObject* earth = solarScene->getObjects()[1];
                TransformComposite* earthComp = dynamic_cast<TransformComposite*>(
                    earth->getTransformation()
                );
                if (earthComp) {
                    earthComp->clear();
                    earthComp->addTransformation(new Rotate(earthAngle, 0.0f, 1.0f, 0.0f));
                    earthComp->addTransformation(new Translate(1.2f, 0.0f, 0.0f));
                    earthComp->addTransformation(new Scale(0.15f, 0.15f, 0.15f));
                }

                // Moon (index 2)
                DrawableObject* moon = solarScene->getObjects()[2];
                TransformComposite* moonComp = dynamic_cast<TransformComposite*>(
                    moon->getTransformation()
                );
                if (moonComp) {
                    moonComp->clear();

                    // Phase 1: Follow Earth around Sun
                    moonComp->addTransformation(new Rotate(earthAngle, 0.0f, 1.0f, 0.0f));
                    moonComp->addTransformation(new Translate(1.2f, 0.0f, 0.0f));

                    // Phase 2: Orbit around Earth
                    moonComp->addTransformation(new Rotate(moonAngle, 0.0f, 1.0f, 0.0f));
                    moonComp->addTransformation(new Translate(0.4f, 0.0f, 0.0f));

                    // Phase 3: Size
                    moonComp->addTransformation(new Scale(0.05f, 0.05f, 0.05f));
                }
            }
        }

        // ============================================
        // SCENE 4: ROTATING TRIANGLE
        // ============================================
        if (sceneManager->getActiveSceneIndex() == 3) {
            float rotationAngle = fmod((float)glfwGetTime() * 60.0f, 360.0f);  // 60°/s

            Scene* scene4 = sceneManager->getActiveScene();
            if (scene4) {
                DrawableObject* obj = scene4->getObjects()[0];
                TransformComposite* comp = dynamic_cast<TransformComposite*>(
                    obj->getTransformation()
                );
                if (comp) {
                    comp->clear();
                    comp->addTransformation(new Rotate(rotationAngle, 0.0f, 0.0f, 1.0f));
                }
            }
        }

        sceneManager->drawActiveScene();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}