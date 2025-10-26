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
#include "Controller.h"
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
#include "Light.h"

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

    // BOD 3c: Framebuffer resize callback
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, Application::framebufferSizeCallback);

    glEnable(GL_DEPTH_TEST);
}

void Application::createShaders()
{
    printf("\n========================================\n");
    printf("CREATING SHADERS FROM FILES\n");
    printf("========================================\n");

    // ========================================
    // STATIC CAMERA SHADERY
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

    // Pripoj statickú kameru k základným shaderom
    cameraStatic->attach(shaderProgram1);
    cameraStatic->attach(shaderProgram2);

    printf("✅ Static camera shaders created from files\n");

    // ========================================
    // LIGHTING SHADERY
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

    std::vector<Shader*> phongShaders = {lightingVertex, phongFragment};
    shaderProgramPhong = new ShaderProgram(phongShaders);

    std::vector<Shader*> lambertShaders = {lightingVertex, lambertFragment};
    shaderProgramLambert = new ShaderProgram(lambertShaders);

    std::vector<Shader*> constantShaders = {lightingVertex, constantFragment};
    shaderProgramConstant = new ShaderProgram(constantShaders);

    std::vector<Shader*> blinnShaders = {lightingVertex, blinnFragment};
    shaderProgramBlinn = new ShaderProgram(blinnShaders);

    // Pripoj OBE kamery k lighting shaderom (používajú sa v rôznych scénach)
    cameraStatic->attach(shaderProgramPhong);
    cameraStatic->attach(shaderProgramLambert);
    cameraStatic->attach(shaderProgramConstant);
    cameraStatic->attach(shaderProgramBlinn);

    cameraDynamic->attach(shaderProgramPhong);
    cameraDynamic->attach(shaderProgramLambert);
    cameraDynamic->attach(shaderProgramConstant);
    cameraDynamic->attach(shaderProgramBlinn);

    printf("✅ Lighting shaders created: Phong, Lambert, Constant, Blinn\n");

    // ========================================
    // DYNAMIC CAMERA SHADERY (Ground/Path)
    // ========================================
    printf("\n--- Creating forest ground/path shaders from files ---\n");

    Shader* simpleVertex = new Shader(GL_VERTEX_SHADER,
        new FileShaderLoader("../src/shaders/simple_vertex.vert"));

    Shader* groundFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/ground_fragment.frag"));

    Shader* pathFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/path_fragment.frag"));

    std::vector<Shader*> groundShaders = {simpleVertex, groundFragment};
    shaderProgramGround = new ShaderProgram(groundShaders);

    std::vector<Shader*> pathShaders = {simpleVertex, pathFragment};
    shaderProgramPath = new ShaderProgram(pathShaders);

    // Ground/Path shadery používajú LEN dynamickú kameru (scéna 6 - les)
    cameraDynamic->attach(shaderProgramGround);
    cameraDynamic->attach(shaderProgramPath);

    printf("✅ Ground/Path shaders created (constant colors, no lighting)\n");

    // ========================================
    // BOD 3b: BACKFACE TEST SHADERY
    // ========================================
    printf("\n--- Creating backface test shaders ---\n");

    Shader* phongWrongFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/phong_wrong_fragment.frag"));

    Shader* phongCorrectFragment = new Shader(GL_FRAGMENT_SHADER,
        new FileShaderLoader("../src/shaders/phong_correct_fragment.frag"));

    std::vector<Shader*> phongWrongShaders = {lightingVertex, phongWrongFragment};
    shaderProgramPhongWrong = new ShaderProgram(phongWrongShaders);

    std::vector<Shader*> phongCorrectShaders = {lightingVertex, phongCorrectFragment};
    shaderProgramPhongCorrect = new ShaderProgram(phongCorrectShaders);

    // Backface test shadery používajú LEN statickú kameru (scéna 7)
    cameraStatic->attach(shaderProgramPhongWrong);
    cameraStatic->attach(shaderProgramPhongCorrect);

    printf("✅ Backface test shaders created\n");

    // ========================================
    // ✅ KRITICKÉ: INICIALIZÁCIA ASPECT RATIO
    // ========================================
    // Učiteľ (0:03): "Znáš, když zmieníte okno, musíte zavolať ten viewport
    //                 a musíte update-ovať v perspektívnej matici."
    //
    // Všetky shadery sú pripojené k kamerám cez Observer pattern.
    // Teraz inicializujeme aspect ratio a notifikujeme VŠETKY shadery naraz.
    // ========================================

    printf("\n--- Initializing camera aspect ratios ---\n");

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspectRatio = (float)width / (float)height;

    // Statická kamera - nastaví aspect ratio a oznámi to VŠETKÝM pripojeným shaderom
    cameraStatic->setAspectRatio(aspectRatio);
    cameraStatic->flushPendingNotifications();
    printf("  Static camera: aspect %.2f → notified %d shaders\n",
           aspectRatio, 6);  // shaderProgram1, 2, Phong, Lambert, Constant, Blinn, PhongWrong, PhongCorrect

    // Dynamická kamera - nastaví aspect ratio a oznámi to VŠETKÝM pripojeným shaderom
    cameraDynamic->setAspectRatio(aspectRatio);
    cameraDynamic->flushPendingNotifications();
    printf("  Dynamic camera: aspect %.2f → notified %d shaders\n",
           aspectRatio, 6);  // Phong, Lambert, Constant, Blinn, Ground, Path

    printf("\n========================================\n");
    printf("✅ ALL SHADERS CREATED FROM FILES!\n");
    printf("   Window: %dx%d (aspect: %.2f)\n", width, height, aspectRatio);
    printf("   Static camera observers: 8\n");
    printf("   Dynamic camera observers: 6\n");
    printf("========================================\n\n");

    // ========================================
    // VYTVORENIE CONTROLLERA
    // ========================================
    // Controller sa vytvára AŽ TERAZ, pretože potrebuje prístup k shaderom
    // pre správnu aktualizáciu svetiel pri prepínaní scén
    // ========================================

    controller = new Controller(
        window, cameraStatic, cameraDynamic, sceneManager,
        shaderProgramPhong, shaderProgramLambert, shaderProgramBlinn,
        shaderProgramPhongWrong, shaderProgramPhongCorrect
    );
    printf("Controller created with shader access\n");
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

    SceneFactory* factory = new SceneFactory(
        // Modely
        model1, model2, model3, modelSuziFlat, modelSuziSmooth,
        modelTree, modelBushes, modelGift, modelPlain, modelBench,

        // Základné shadery
        shaderProgram1, shaderProgram2,

        // Lighting shadery
        shaderProgramPhong, shaderProgramLambert,
        shaderProgramConstant, shaderProgramBlinn,

        // Ground/Path shadery (bez osvetlenia)
        shaderProgramGround, shaderProgramPath,

        // BOD 3b: Backface test shadery
        shaderProgramPhongWrong, shaderProgramPhongCorrect
    );

    sceneManager->addScene(factory->createPhongTestScene());          // Scéna 1
    sceneManager->addScene(factory->createSolarSystemScene());        // Scéna 2
    sceneManager->addScene(factory->createBothObjectsScene());        // Scéna 3
    sceneManager->addScene(factory->createRotatingTriangleScene());   // Scéna 4
    sceneManager->addScene(factory->createLightingDemoScene());       // Scéna 5
    sceneManager->addScene(factory->createForestScene());             // Scéna 6
    sceneManager->addScene(factory->createBackfaceLightingTest());    // Scéna 7

    delete factory;

    printf("========================================\n");
    printf("ALL SCENES CREATED!\n");
    printf("Total scenes: %d\n", sceneManager->getSceneCount());
    printf("Press keys 1-7 to switch scenes\n");
    printf("========================================\n\n");
}

void Application::setupSceneLights()
{
    printf("\n========================================\n");
    printf("SETTING UP SCENE LIGHTS\n");
    printf("========================================\n");

    for (int i = 0; i < sceneManager->getSceneCount(); i++) {
        Scene* scene = sceneManager->getScene(i);
        if (!scene) continue;

        printf("\nScene %d (index %d):\n", i + 1, i);

        std::vector<Light*>& lights = scene->getLights();
        if (lights.empty()) {
            printf("  - No lights\n");
            continue;
        }

        printf("  - %d lights detected\n", (int)lights.size());

        // Pripoj observerov pre každé svetlo
        for (Light* light : lights) {
            if (!light) continue;

            // Scéna 6 = les (index 5)
            if (i == 5) {
                light->attach(shaderProgramLambert);
                light->attach(shaderProgramPhong);
                light->attach(shaderProgramBlinn);
            }
            // Scéna 7 = backface test (index 6)
            else if (i == 6) {
                light->attach(shaderProgramPhongWrong);
                light->attach(shaderProgramPhongCorrect);
            }
            // Ostatné scény
            else {
                light->attach(shaderProgramPhong);
                light->attach(shaderProgramLambert);
                light->attach(shaderProgramBlinn);
            }
        }

        // Pošli svetlá do shaderov
        if (i == 5) {
            // Les používa všetky tri shadery (index 5)
            shaderProgramLambert->setLights(lights);
            shaderProgramPhong->setLights(lights);
            shaderProgramBlinn->setLights(lights);
            printf("  ✅ Sent to forest shaders (Lambert, Phong, Blinn)\n");
        }
        else if (i == 6) {
            // Backface test (index 6)
            shaderProgramPhongWrong->setLights(lights);
            shaderProgramPhongCorrect->setLights(lights);
            printf("  ✅ Sent to backface test shaders (PhongWrong, PhongCorrect)\n");
        }
        else {
            // Ostatné scény
            shaderProgramPhong->setLights(lights);
            shaderProgramLambert->setLights(lights);
            shaderProgramBlinn->setLights(lights);
            printf("  ✅ Sent to lighting shaders\n");
        }

        // Notifikuj všetky svetlá
        for (Light* light : lights) {
            if (light) {
                light->notifyAll();
            }
        }
        printf("  ✅ Initial notifications sent\n");
    }

    // Prepni na prvú scénu
    sceneManager->switchToScene(0);

    printf("\n========================================\n");
    printf("✅ ALL SCENE LIGHTS CONFIGURED\n");
    printf("========================================\n\n");
}

// ========================================
// BOD 3c: FRAMEBUFFER RESIZE
// ========================================
void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->updateViewport(width, height);
    }
}

void Application::updateViewport(int width, int height)
{
    glViewport(0, 0, width, height);

    float aspectRatio = (float)width / (float)height;

    // ✅ OPTIMALIZOVANÉ: Aktualizuj len aktívnu kameru
    Camera* activeCamera = (sceneManager->getActiveSceneIndex() == 5)
                            ? cameraDynamic
                            : cameraStatic;

    activeCamera->setAspectRatio(aspectRatio);

    printf("✅ Viewport updated: %dx%d (aspect: %.2f) - notification pending\n",
           width, height, aspectRatio);
}

void Application::run() {
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        // ✅ Notifikuj len aktívnu kameru
        Camera* activeCamera = (sceneManager->getActiveSceneIndex() == 5)
                                ? cameraDynamic
                                : cameraStatic;

        activeCamera->flushPendingNotifications();

        // Kurzor
        if (sceneManager->getActiveSceneIndex() == 5)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        controller->updateMovement(deltaTime);

        // ============================================
        // SCENE 2: SOLAR SYSTEM ANIMATION
        // ============================================
        if (sceneManager->getActiveSceneIndex() == 1) {
            float currentTime = (float)glfwGetTime();
            float earthAngle = fmod(currentTime * 30.0f, 360.0f);
            float moonAngle = fmod(currentTime * 48.0f, 360.0f);

            Scene* solarScene = sceneManager->getActiveScene();
            if (solarScene && solarScene->getObjects().size() >= 3) {

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

                DrawableObject* moon = solarScene->getObjects()[2];
                TransformComposite* moonComp = dynamic_cast<TransformComposite*>(
                    moon->getTransformation()
                );
                if (moonComp) {
                    moonComp->clear();
                    moonComp->addTransformation(new Rotate(earthAngle, 0.0f, 1.0f, 0.0f));
                    moonComp->addTransformation(new Translate(1.2f, 0.0f, 0.0f));
                    moonComp->addTransformation(new Rotate(moonAngle, 0.0f, 1.0f, 0.0f));
                    moonComp->addTransformation(new Translate(0.4f, 0.0f, 0.0f));
                    moonComp->addTransformation(new Scale(0.05f, 0.05f, 0.05f));
                }
            }
        }

        // ============================================
        // SCENE 4: ROTATING TRIANGLE
        // ============================================
        if (sceneManager->getActiveSceneIndex() == 3) {
            float rotationAngle = fmod((float)glfwGetTime() * 60.0f, 360.0f);

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

        // ============================================
        // SCENE 6: FIREFLIES ANIMATION (index 5!)
        // ============================================
        if (sceneManager->getActiveSceneIndex() == 5) {
            Scene* forestScene = sceneManager->getActiveScene();

            if (forestScene && forestScene->getLightCount() >= 4) {
                float time = (float)glfwGetTime();

                // SVETLUŠKA 1
                Light* firefly1 = forestScene->getLights()[0];
                float x1 = -3.0f + 2.0f * sin(time * 0.5f);
                float y1 = 1.5f + 0.5f * cos(time * 0.7f);
                float z1 = 2.0f + 1.5f * sin(time * 0.3f);

                TransformComposite* newTransform1 = new TransformComposite();
                newTransform1->addTransformation(new Translate(x1, y1, z1));
                firefly1->setTransformation(newTransform1);

                DrawableObject* fireflyVisual1 = forestScene->getObjects()[119];
                TransformComposite* visualTransform1 = dynamic_cast<TransformComposite*>(
                    fireflyVisual1->getTransformation()
                );
                if (visualTransform1) {
                    visualTransform1->clear();
                    visualTransform1->addTransformation(new Translate(x1, y1, z1));
                    visualTransform1->addTransformation(new Scale(0.05f, 0.05f, 0.05f));
                }

                // SVETLUŠKA 2
                Light* firefly2 = forestScene->getLights()[1];
                float radius = 3.0f;
                float x2 = radius * cos(time * 0.6f);
                float y2 = 1.2f + 0.3f * sin(time * 0.9f);
                float z2 = radius * sin(time * 0.6f);

                TransformComposite* newTransform2 = new TransformComposite();
                newTransform2->addTransformation(new Translate(x2, y2, z2));
                firefly2->setTransformation(newTransform2);

                DrawableObject* fireflyVisual2 = forestScene->getObjects()[120];
                TransformComposite* visualTransform2 = dynamic_cast<TransformComposite*>(
                    fireflyVisual2->getTransformation()
                );
                if (visualTransform2) {
                    visualTransform2->clear();
                    visualTransform2->addTransformation(new Translate(x2, y2, z2));
                    visualTransform2->addTransformation(new Scale(0.05f, 0.05f, 0.05f));
                }

                // SVETLUŠKA 3
                Light* firefly3 = forestScene->getLights()[2];
                float y3 = 2.0f + 1.0f * sin(time * 0.8f);
                float x3 = 0.5f * cos(time * 0.4f);
                float z3 = 0.5f * sin(time * 0.4f);

                TransformComposite* newTransform3 = new TransformComposite();
                newTransform3->addTransformation(new Translate(x3, y3, z3));
                firefly3->setTransformation(newTransform3);

                DrawableObject* fireflyVisual3 = forestScene->getObjects()[121];
                TransformComposite* visualTransform3 = dynamic_cast<TransformComposite*>(
                    fireflyVisual3->getTransformation()
                );
                if (visualTransform3) {
                    visualTransform3->clear();
                    visualTransform3->addTransformation(new Translate(x3, y3, z3));
                    visualTransform3->addTransformation(new Scale(0.05f, 0.05f, 0.05f));
                }

                // SVETLUŠKA 4
                Light* firefly4 = forestScene->getLights()[3];
                float x4 = -2.0f + 2.0f * sin(time * 0.5f);
                float y4 = 1.8f + 0.4f * cos(time * 1.0f);
                float z4 = -6.0f + 1.5f * sin(time * 1.0f);

                TransformComposite* newTransform4 = new TransformComposite();
                newTransform4->addTransformation(new Translate(x4, y4, z4));
                firefly4->setTransformation(newTransform4);

                DrawableObject* fireflyVisual4 = forestScene->getObjects()[122];
                TransformComposite* visualTransform4 = dynamic_cast<TransformComposite*>(
                    fireflyVisual4->getTransformation()
                );
                if (visualTransform4) {
                    visualTransform4->clear();
                    visualTransform4->addTransformation(new Translate(x4, y4, z4));
                    visualTransform4->addTransformation(new Scale(0.05f, 0.05f, 0.05f));
                }
            }
        }

        sceneManager->drawActiveScene();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}


