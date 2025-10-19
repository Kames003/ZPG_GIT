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
    cameraStatic->attachObserver(shaderProgram1);
    cameraStatic->attachObserver(shaderProgram2);

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

    cameraStatic->notifyObservers();

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
    cameraDynamic->attachObserver(shaderProgramTree);
    cameraDynamic->attachObserver(shaderProgramBush);
    cameraDynamic->attachObserver(shaderProgramGround);
    cameraDynamic->attachObserver(shaderProgramPath);
    cameraDynamic->attachObserver(shaderProgramBench);

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

    cameraDynamic->notifyObservers();

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
    mainLight->attachObserver(shaderProgramPhong);
    mainLight->attachObserver(shaderProgramLambert);
    mainLight->attachObserver(shaderProgramBlinn);

    // Pripoj kamery k lighting shaderom
    cameraStatic->attachObserver(shaderProgramPhong);
    cameraStatic->attachObserver(shaderProgramLambert);
    cameraStatic->attachObserver(shaderProgramConstant);
    cameraStatic->attachObserver(shaderProgramBlinn);

    cameraDynamic->attachObserver(shaderProgramPhong);
    cameraDynamic->attachObserver(shaderProgramLambert);
    cameraDynamic->attachObserver(shaderProgramConstant);
    cameraDynamic->attachObserver(shaderProgramBlinn);

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
    mainLight->notifyObservers();
    printf("✓ Light notifications sent\n");

    cameraStatic->notifyObservers();
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
    // ============================================
    // SCENE 1: PHONG LIGHTING TEST
    // ============================================
    Scene* scene1 = new Scene();
    Translate* t1 = new Translate(0.0f, 0.0f, 0.0f);

    DrawableObject* obj1 = new DrawableObject(
        model1,
        t1,
        shaderProgramPhong,
        glm::vec3(0.8f, 0.2f, 0.2f)
    );

    scene1->addObject(obj1);
    sceneManager->addScene(scene1);
    printf("Scene 1: Phong lighting test (red triangle)\n");

    // ============================================
    // SCENE 2: SOLAR SYSTEM (Sun, Earth, Moon)
    // ============================================
    Scene* solarScene = new Scene();

    printf("\n=== Creating Scene 2: Solar System ===\n");

    // 1. SLNKO
    TransformComposite* sunTransform = new TransformComposite();
    sunTransform->addTransformation(new Translate(0.0f, 0.0f, 0.0f));
    sunTransform->addTransformation(new Scale(0.3f, 0.3f, 0.3f));

    DrawableObject* sun = new DrawableObject(
        model3,
        sunTransform,
        shaderProgramConstant,
        glm::vec3(1.0f, 0.9f, 0.0f)
    );
    solarScene->addObject(sun);
    printf("Sun created (yellow, center)\n");

    // 2. ZEM
    TransformComposite* earthOrbit = new TransformComposite();
    earthOrbit->addTransformation(new Rotate(0.0f, 0.0f, 1.0f, 0.0f));
    earthOrbit->addTransformation(new Translate(1.2f, 0.0f, 0.0f));
    earthOrbit->addTransformation(new Scale(0.15f, 0.15f, 0.15f));

    DrawableObject* earth = new DrawableObject(
        model3,
        earthOrbit,
        shaderProgramPhong,
        glm::vec3(0.2f, 0.4f, 0.8f)
    );
    solarScene->addObject(earth);
    printf("Earth created (blue, orbiting Sun)\n");

    // 3. MESIAC
    TransformComposite* moonOrbit = new TransformComposite();
    moonOrbit->addTransformation(new Rotate(0.0f, 0.0f, 1.0f, 0.0f));
    moonOrbit->addTransformation(new Translate(1.2f, 0.0f, 0.0f));
    moonOrbit->addTransformation(new Rotate(0.0f, 0.0f, 1.0f, 0.0f));
    moonOrbit->addTransformation(new Translate(0.4f, 0.0f, 0.0f));
    moonOrbit->addTransformation(new Scale(0.05f, 0.05f, 0.05f));

    DrawableObject* moon = new DrawableObject(
        model3,
        moonOrbit,
        shaderProgramLambert,
        glm::vec3(0.7f, 0.7f, 0.7f)
    );
    solarScene->addObject(moon);
    printf("Moon created (gray, orbiting Earth)\n");

    sceneManager->addScene(solarScene);
    printf("=== Solar System Scene 2 Complete ===\n\n");

    // ============================================
    // SCENE 3: BOTH OBJECTS
    // ============================================
    Scene* scene3 = new Scene();
    Translate* t4 = new Translate(-0.5f, 0.0f, 0.0f);
    DrawableObject* obj3 = new DrawableObject(model1, t4, shaderProgram1);
    Translate* t5 = new Translate(0.5f, 0.0f, 0.0f);
    DrawableObject* obj4 = new DrawableObject(model2, t5, shaderProgram2);
    scene3->addObject(obj3);
    scene3->addObject(obj4);
    sceneManager->addScene(scene3);

    // ============================================
    // SCENE 4: ROTATING TRIANGLE
    // ============================================
    Scene* scene4 = new Scene();
    TransformComposite* rotatingComposite = new TransformComposite();
    rotatingComposite->addTransformation(new Rotate(0.0f, 0.0f, 0.0f, 1.0f));
    DrawableObject* obj5 = new DrawableObject(model1, rotatingComposite, shaderProgram1);
    scene4->addObject(obj5);
    sceneManager->addScene(scene4);

    // ============================================
    // SCENE 5: LIGHTING DEMO (4 green spheres)
    // ============================================
    Scene* scene5 = new Scene();

    printf("\n=== Creating Scene 5: Lighting Demo ===\n");

    glm::vec3 greenColor(0.2f, 0.8f, 0.2f);

    // Sphere 1 - Top
    TransformComposite* sphere1Transform = new TransformComposite();
    sphere1Transform->addTransformation(new Translate(0.0f, 0.6f, 0.0f));
    sphere1Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    DrawableObject* sphere1 = new DrawableObject(model3, sphere1Transform, shaderProgramPhong, greenColor);
    scene5->addObject(sphere1);

    // Sphere 2 - Right
    TransformComposite* sphere2Transform = new TransformComposite();
    sphere2Transform->addTransformation(new Translate(0.6f, 0.0f, 0.0f));
    sphere2Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    DrawableObject* sphere2 = new DrawableObject(model3, sphere2Transform, shaderProgramPhong, greenColor);
    scene5->addObject(sphere2);

    // Sphere 3 - Bottom
    TransformComposite* sphere3Transform = new TransformComposite();
    sphere3Transform->addTransformation(new Translate(0.0f, -0.6f, 0.0f));
    sphere3Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    DrawableObject* sphere3 = new DrawableObject(model3, sphere3Transform, shaderProgramPhong, greenColor);
    scene5->addObject(sphere3);

    // Sphere 4 - Left
    TransformComposite* sphere4Transform = new TransformComposite();
    sphere4Transform->addTransformation(new Translate(-0.6f, 0.0f, 0.0f));
    sphere4Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    DrawableObject* sphere4 = new DrawableObject(model3, sphere4Transform, shaderProgramPhong, greenColor);
    scene5->addObject(sphere4);

    sceneManager->addScene(scene5);
    printf("✓ Lighting demo created\n\n");

    // ============================================
    // SCENE 6: COMPLEX SCENE (20+ objects)
    // ============================================
    Scene* scene6 = new Scene();

    // 4 suzi flat - corners
    TransformComposite* suzi1 = new TransformComposite();
    suzi1->addTransformation(new Translate(-0.7f, 0.7f, 0.0f));
    suzi1->addTransformation(new Scale(0.12f, 0.12f, 0.12f));
    scene6->addObject(new DrawableObject(modelSuziFlat, suzi1, shaderProgram1));

    TransformComposite* suzi2 = new TransformComposite();
    suzi2->addTransformation(new Translate(0.7f, 0.7f, 0.0f));
    suzi2->addTransformation(new Scale(0.12f, 0.12f, 0.12f));
    scene6->addObject(new DrawableObject(modelSuziFlat, suzi2, shaderProgram1));

    TransformComposite* suzi3 = new TransformComposite();
    suzi3->addTransformation(new Translate(-0.7f, -0.7f, 0.0f));
    suzi3->addTransformation(new Scale(0.12f, 0.12f, 0.12f));
    scene6->addObject(new DrawableObject(modelSuziFlat, suzi3, shaderProgram1));

    TransformComposite* suzi4 = new TransformComposite();
    suzi4->addTransformation(new Translate(0.7f, -0.7f, 0.0f));
    suzi4->addTransformation(new Scale(0.12f, 0.12f, 0.12f));
    scene6->addObject(new DrawableObject(modelSuziFlat, suzi4, shaderProgram1));

    // 5 trees - top row
    for (int i = 0; i < 5; i++) {
        TransformComposite* tr = new TransformComposite();
        tr->addTransformation(new Translate(-0.6f + i * 0.3f, 0.35f, 0.0f));
        tr->addTransformation(new Scale(0.03f, 0.03f, 0.03f));
        scene6->addObject(new DrawableObject(modelTree, tr, shaderProgram2));
    }

    // 5 bushes - bottom row
    for (int i = 0; i < 5; i++) {
        TransformComposite* bu = new TransformComposite();
        bu->addTransformation(new Translate(-0.5f + i * 0.25f, -0.35f, 0.0f));
        bu->addTransformation(new Scale(0.15f, 0.15f, 0.15f));
        scene6->addObject(new DrawableObject(modelBushes, bu, shaderProgram1));
    }

    // 4 balls - edges
    float edgePositions[][2] = {{0.9f, 0.0f}, {-0.9f, 0.0f}, {0.0f, 0.9f}, {0.0f, -0.9f}};
    for (int i = 0; i < 4; i++) {
        TransformComposite* sph = new TransformComposite();
        sph->addTransformation(new Translate(edgePositions[i][0], edgePositions[i][1], 0.0f));
        sph->addTransformation(new Scale(0.07f, 0.07f, 0.07f));
        scene6->addObject(new DrawableObject(model3, sph, shaderProgram2));
    }

    // 4 gifts - middle
    float giftPositions[][2] = {{-0.15f, 0.1f}, {0.15f, 0.1f}, {-0.15f, -0.1f}, {0.15f, -0.1f}};
    for (int i = 0; i < 4; i++) {
        TransformComposite* gi = new TransformComposite();
        gi->addTransformation(new Translate(giftPositions[i][0], giftPositions[i][1], 0.0f));
        gi->addTransformation(new Scale(0.18f, 0.18f, 0.18f));
        scene6->addObject(new DrawableObject(modelGift, gi, shaderProgram1));
    }

    sceneManager->addScene(scene6);

    // ============================================
    // SCENE 7: FOREST WITH LIGHTING
    // ============================================
    Scene* forestScene = new Scene();

    printf("\n=== Creating Forest Scene 7 (with lighting) ===\n");

    // Ground
    TransformComposite* groundTransform = new TransformComposite();
    groundTransform->addTransformation(new Translate(0.0f, 0.0f, 0.0f));
    groundTransform->addTransformation(new Scale(20.0f, 1.0f, 20.0f));
    forestScene->addObject(new DrawableObject(modelPlain, groundTransform, shaderProgramGround));

    // Path
    TransformComposite* pathTransform = new TransformComposite();
    pathTransform->addTransformation(new Translate(0.0f, 0.01f, 0.0f));
    pathTransform->addTransformation(new Scale(2.0f, 1.0f, 20.0f));
    forestScene->addObject(new DrawableObject(modelPlain, pathTransform, shaderProgramPath));

    // Benches (3x)
    float benchPositions[][3] = {{-1.5f, 0.0f, -5.0f}, {1.5f, 0.0f, 0.0f}, {-1.5f, 0.0f, 5.0f}};
    for (int i = 0; i < 3; i++) {
        TransformComposite* bench = new TransformComposite();
        bench->addTransformation(new Translate(benchPositions[i][0], benchPositions[i][1], benchPositions[i][2]));
        bench->addTransformation(new Scale(0.8f, 0.8f, 0.8f));
        bench->addTransformation(new Rotate(90.0f, 0.0f, 1.0f, 0.0f));
        forestScene->addObject(new DrawableObject(modelBench, bench, shaderProgramBlinn, glm::vec3(0.5f, 0.3f, 0.1f)));
    }

    // Trees (8x8 grid)
    int treeCount = 0;
    float gridSpacing = 2.5f;
    int gridSize = 8;

    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            float x = -9.0f + col * gridSpacing;
            float z = -9.0f + row * gridSpacing;

            TransformComposite* treeTransform = new TransformComposite();
            treeTransform->addTransformation(new Translate(x, 0.0f, z));
            treeTransform->addTransformation(new Scale(0.3f, 0.3f, 0.3f));
            forestScene->addObject(new DrawableObject(
                modelTree,
                treeTransform,
                shaderProgramLambert,
                glm::vec3(0.11f, 0.38f, 0.0f)
            ));
            treeCount++;
        }
    }
    printf("Trees created: %d - Lambert lighting\n", treeCount);

    // Bushes (5x10 grid)
    int bushCount = 0;
    float bushSpacingX = 2.0f;
    float bushSpacingZ = 4.0f;
    int bushRows = 5;
    int bushCols = 10;

    for (int row = 0; row < bushRows; row++) {
        for (int col = 0; col < bushCols; col++) {
            float x = -9.0f + col * bushSpacingX;
            float z = -8.0f + row * bushSpacingZ;

            TransformComposite* bushTransform = new TransformComposite();
            bushTransform->addTransformation(new Translate(x, -0.001f, z));
            bushTransform->addTransformation(new Scale(0.5f, 0.5f, 0.5f));
            forestScene->addObject(new DrawableObject(
                modelBushes,
                bushTransform,
                shaderProgramPhong,
                glm::vec3(0.2f, 0.8f, 0.2f)
            ));
            bushCount++;
        }
    }
    printf("Bushes created: %d - Phong lighting\n", bushCount);

    sceneManager->addScene(forestScene);

    printf("=== Forest Scene 7 Complete ===\n");
    printf("Total: 1 ground, 1 path, %d trees, %d bushes, 3 benches\n\n", treeCount, bushCount);

    printf("Scenes created successfully!\n");
    printf("Scene 1: Phong lighting test (red triangle)\n");
    printf("Scene 2: Solar System (Sun, Earth, Moon)\n");
    printf("Scene 3: Both objects\n");
    printf("Scene 4: Rotating triangle\n");
    printf("Scene 5: 4 spheres lighting demo\n");
    printf("Scene 6: Complex scene with 22 objects\n");
    printf("Scene 7: Procedural forest (camera controls: WSAD + mouse)\n");
    printf("Press keys 1-7 to switch scenes\n");
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
            static float earthAngle = 0.0f;
            static float moonAngle = 0.0f;

            earthAngle += 0.5f;
            moonAngle += 0.8f;

            if (earthAngle > 360.0f) earthAngle -= 360.0f;
            if (moonAngle > 360.0f) moonAngle -= 360.0f;

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
            rotationAngle += 1.0f;
            if (rotationAngle > 360.0f) rotationAngle -= 360.0f;

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