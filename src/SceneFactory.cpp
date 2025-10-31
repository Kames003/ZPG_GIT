#include "SceneFactory.h"
#include "Scene.h"
#include "DrawableObject.h"
#include "TransformComposite.h"
#include "Translate.h"
#include "Rotate.h"
#include "Scale.h"
#include "Model.h"
#include "ShaderProgram.h"
#include <cstdio>
#include <glm/glm.hpp>
#include "Light.h"

// ========================================
// CONSTRUCTOR
// ========================================
SceneFactory::SceneFactory(
    Model* m1, Model* m2, Model* m3, Model* suziFlat, Model* suziSmooth,
    Model* tree, Model* bushes, Model* gift, Model* plain, Model* bench,
    ShaderProgram* sp1, ShaderProgram* sp2,
    ShaderProgram* spPhong, ShaderProgram* spLambert,
    ShaderProgram* spConstant, ShaderProgram* spBlinn,
    ShaderProgram* spGround, ShaderProgram* spPath,
    ShaderProgram* spPhongWrong, ShaderProgram* spPhongCorrect)

    : model1(m1), model2(m2), model3(m3),
      modelSuziFlat(suziFlat), modelSuziSmooth(suziSmooth),
      modelTree(tree), modelBushes(bushes), modelGift(gift),
      modelPlain(plain), modelBench(bench),
      shaderProgram1(sp1), shaderProgram2(sp2),
      shaderProgramPhong(spPhong), shaderProgramLambert(spLambert),
      shaderProgramConstant(spConstant), shaderProgramBlinn(spBlinn),
      shaderProgramGround(spGround), shaderProgramPath(spPath),
      shaderProgramPhongWrong(spPhongWrong),
      shaderProgramPhongCorrect(spPhongCorrect)
{
    printf("SceneFactory initialized\n");
}

    // ========================================
    // SCENE 1: PHONG TEST
    // ========================================

Scene* SceneFactory::createPhongTestScene() {
    printf("Creating Scene 1: Phong Test\n");

    Scene* scene = new Scene();

    Translate* t1 = new Translate(0.0f, 0.0f, 0.0f);
    DrawableObject* obj1 = new DrawableObject(
        model1, t1, shaderProgramPhong, glm::vec3(0.8f, 0.2f, 0.2f)
    );
    scene->addObject(obj1);

    // vytvárame si transformačný objekt
    // týmto objektom definujeme pozíciu svetla v 3d priestore

    TransformComposite* light1Transform = new TransformComposite();
    light1Transform->addTransformation(new Translate(0.0f, 0.0f, 2.0f));

    Light* light1 = new Light(
        light1Transform, // pozícia svetla ta je daná našim light1Transform
        glm::vec3(1.0f, 1.0f, 1.0f),  // farba R G B
        1.0f // intenzita
    );
    light1->attach(shaderProgramPhong); // pripojenie svetla k shaderu
    scene->addLight(light1);

    TransformComposite* light2Transform = new TransformComposite();
    light2Transform->addTransformation(new Translate(5.0f, 3.0f, 2.0f));

    Light* light2 = new Light(
        light2Transform,
        glm::vec3(1.0f, 0.5f, 0.5f),
        0.8f
    );
    light2->attach(shaderProgramPhong);
    scene->addLight(light2);

    TransformComposite* light3Transform = new TransformComposite();
    light3Transform->addTransformation(new Translate(-5.0f, 3.0f, 2.0f));

    Light* light3 = new Light(
        light3Transform,
        glm::vec3(0.5f, 0.5f, 1.0f),
        0.8f
    );
    light3->attach(shaderProgramPhong);
    scene->addLight(light3);

    printf("Scene 1 created: 1 object, 3 lights\n");
    // poslanie svetiel do shadera
    shaderProgramPhong->setLights(scene->getLights());

    return scene;
}

    // ========================================
    // SCENE 2: SOLAR SYSTEM
    // ========================================

Scene* SceneFactory::createSolarSystemScene() {
    printf("Creating Scene 2: Solar System\n");

    Scene* scene = new Scene();
    TransformComposite* sunTransform = new TransformComposite();
    sunTransform->addTransformation(new Translate(0.0f, 0.0f, 0.0f));
    sunTransform->addTransformation(new Scale(0.3f, 0.3f, 0.3f));
    DrawableObject* sun = new DrawableObject(
        model3, sunTransform, shaderProgramConstant, glm::vec3(1.0f, 0.9f, 0.0f)
    );
    scene->addObject(sun);

    TransformComposite* earthOrbit = new TransformComposite();
    earthOrbit->addTransformation(new Rotate(0.0f, 0.0f, 1.0f, 0.0f));
    earthOrbit->addTransformation(new Translate(1.2f, 0.0f, 0.0f));
    earthOrbit->addTransformation(new Scale(0.15f, 0.15f, 0.15f));
    DrawableObject* earth = new DrawableObject(
        model3, earthOrbit, shaderProgramPhong, glm::vec3(0.2f, 0.4f, 0.8f)
    );
    scene->addObject(earth);

    TransformComposite* moonOrbit = new TransformComposite();
    moonOrbit->addTransformation(new Rotate(0.0f, 0.0f, 1.0f, 0.0f));
    moonOrbit->addTransformation(new Translate(1.2f, 0.0f, 0.0f));
    moonOrbit->addTransformation(new Rotate(0.0f, 0.0f, 1.0f, 0.0f));
    moonOrbit->addTransformation(new Translate(0.4f, 0.0f, 0.0f));
    moonOrbit->addTransformation(new Scale(0.05f, 0.05f, 0.05f));
    DrawableObject* moon = new DrawableObject(
        model3, moonOrbit, shaderProgramLambert, glm::vec3(0.7f, 0.7f, 0.7f)
    );
    scene->addObject(moon);

    TransformComposite* sunLightTransform = new TransformComposite();
    sunLightTransform->addTransformation(new Translate(0.0f, 0.0f, 0.0f));

    Light* sunLight = new Light(
        sunLightTransform,
        glm::vec3(1.0f, 0.9f, 0.7f),
        1.5f
    );
    scene->addLight(sunLight);

    printf("Solar System created: 3 objects, 1 light (Sun)\n");
    return scene;
}

// ========================================
// SCENE 3: BOTH OBJECTS
// ========================================
Scene* SceneFactory::createBothObjectsScene() {
    printf("Creating Scene 3: Both Objects\n");

    Scene* scene = new Scene();

    Translate* t1 = new Translate(-0.5f, 0.0f, 0.0f);
    DrawableObject* obj1 = new DrawableObject(model1, t1, shaderProgram1);
    scene->addObject(obj1);

    Translate* t2 = new Translate(0.5f, 0.0f, 0.0f);
    DrawableObject* obj2 = new DrawableObject(model2, t2, shaderProgram2);
    scene->addObject(obj2);

    return scene;
}

// ========================================
// SCENE 4: ROTATING TRIANGLE
// ========================================
Scene* SceneFactory::createRotatingTriangleScene() {
    printf("Creating Scene 4: Rotating Triangle\n");

    Scene* scene = new Scene();

    TransformComposite* rotatingComposite = new TransformComposite();
    rotatingComposite->addTransformation(new Rotate(0.0f, 0.0f, 0.0f, 1.0f));
    DrawableObject* obj = new DrawableObject(model1, rotatingComposite, shaderProgram1);
    scene->addObject(obj);

    return scene;
}



// ========================================
// SCENE 5: LIGHTING DEMO
// ========================================

Scene* SceneFactory::createLightingDemoScene() {
    printf("\n=== Creating Scene 5: Phong Lighting Test ===\n");

    Scene* scene = new Scene();
    glm::vec3 sphereColor(0.0f, 0.8f, 0.0f);

    // Sphere 1 - Top (kladná Y os)
    TransformComposite* sphere1Transform = new TransformComposite();
    sphere1Transform->addTransformation(new Translate(0.0f, 0.6f, 0.0f));
    sphere1Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    scene->addObject(new DrawableObject(
        model3, sphere1Transform, shaderProgramPhong, sphereColor
    ));

    // Sphere 2 - Right (kladna X os)
    TransformComposite* sphere2Transform = new TransformComposite();
    sphere2Transform->addTransformation(new Translate(0.6f, 0.0f, 0.0f));
    sphere2Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    scene->addObject(new DrawableObject(
        model3, sphere2Transform, shaderProgramPhong, sphereColor
    ));

    // Sphere 3 - Bottom (záporná Y os)
    TransformComposite* sphere3Transform = new TransformComposite();
    sphere3Transform->addTransformation(new Translate(0.0f, -0.6f, 0.0f));
    sphere3Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    scene->addObject(new DrawableObject(
        model3, sphere3Transform, shaderProgramPhong, sphereColor
    ));

    // Sphere 4 - Left (zaporna¡ X os)
    TransformComposite* sphere4Transform = new TransformComposite();
    sphere4Transform->addTransformation(new Translate(-0.6f, 0.0f, 0.0f));
    sphere4Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    scene->addObject(new DrawableObject(
        model3, sphere4Transform, shaderProgramPhong, sphereColor
    ));

    TransformComposite* centerLightTransform = new TransformComposite();
    centerLightTransform->addTransformation(new Translate(0.0f, 0.0f, 0.0f));

    Light* centerLight = new Light(
        centerLightTransform,
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f  // Normálna intenzita
    );
    scene->addLight(centerLight);

    printf("Scene 5 (Phong Test) created:\n");
    printf("  - 4 spheres around center [0,0,0]\n");
    printf("  - 1 point light AT center [0,0,0]\n");
    printf("  - Camera should be ABOVE (looking down)\n");
    printf("  - Expected: Symmetric lighting, subtle highlights\n\n");

    return scene;
}
// ========================================
// SCENE 6: Shader test
// ========================================

Scene* SceneFactory::createBackfaceLightingTest() {
    printf("Creating Scene 8: Backface Lighting Test\n");

    Scene* scene = new Scene();
    TransformComposite* sphereTransform = new TransformComposite();
    sphereTransform->addTransformation(new Translate(0.0f, 0.0f, 0.0f));
    sphereTransform->addTransformation(new Scale(0.5f, 0.5f, 0.5f));

    DrawableObject* sphere = new DrawableObject(
        model3, sphereTransform, shaderProgramPhongCorrect,
        glm::vec3(0.6f, 0.6f, 0.6f)  // Sivá
    );
    scene->addObject(sphere);


    TransformComposite* lightTransform = new TransformComposite();
    lightTransform->addTransformation(new Translate(0.0f, 0.0f, -1.8f));
    Light* backLight = new Light(
        lightTransform,
        glm::vec3(1.0f, 1.0f, 1.0f),
        30.0f
    );
    scene->addLight(backLight);

    printf("\n=== Scene 8: Backface Lighting Test ===\n");
    printf("  Setup:\n");
    printf("    Guľa:     [0.0, 0.0, 0.0] (r=0.5)\n");
    printf("    Svetlo:   [0.0, 0.0, -1.5] (za guľou)\n");
    printf("    Kamera:   [0.0, 0.0, 3.0] (spredu)\n");
    printf("    Intenzita: 12.0\n");
    printf("\n");
    return scene;
}


Scene* SceneFactory::createForestScene() {
    printf("Creating Scene 7: Night Forest with Glowing Fireflies\n");

    Scene* scene = new Scene();
    TransformComposite* groundTransform = new TransformComposite();
    groundTransform->addTransformation(new Translate(0.0f, 0.0f, 0.0f));
    groundTransform->addTransformation(new Scale(20.0f, 1.0f, 20.0f));
    scene->addObject(new DrawableObject(modelPlain, groundTransform, shaderProgramGround));

    TransformComposite* pathTransform = new TransformComposite();
    pathTransform->addTransformation(new Translate(0.0f, 0.01f, 0.0f));
    pathTransform->addTransformation(new Scale(2.0f, 1.0f, 20.0f));
    scene->addObject(new DrawableObject(modelPlain, pathTransform, shaderProgramPath));


    float benchPositions[][3] = {
        {-1.5f, 0.0f, -5.0f}, {1.5f, 0.0f, 0.0f}, {-1.5f, 0.0f, 5.0f}
    };
    for (int i = 0; i < 3; i++) {
        TransformComposite* benchTransform = new TransformComposite();
        benchTransform->addTransformation(new Translate(benchPositions[i][0], benchPositions[i][1], benchPositions[i][2]));
        benchTransform->addTransformation(new Scale(0.8f, 0.8f, 0.8f));
        benchTransform->addTransformation(new Rotate(90.0f, 0.0f, 1.0f, 0.0f));
        scene->addObject(new DrawableObject(modelBench, benchTransform, shaderProgramBlinn, glm::vec3(0.5f, 0.3f, 0.1f)));
    }

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

            scene->addObject(new DrawableObject(
                modelTree, treeTransform, shaderProgramLambert, glm::vec3(0.11f, 0.38f, 0.0f)
            ));
            treeCount++;
        }
    }

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
            
            scene->addObject(new DrawableObject(
                modelBushes, bushTransform, shaderProgramPhong, glm::vec3(0.2f, 0.8f, 0.2f)
            ));
            bushCount++;
        }
    }

    printf("\n--- Creating GLOWING fireflies for NIGHT forest ---\n");

    TransformComposite* firefly1Transform = new TransformComposite();
    firefly1Transform->addTransformation(new Translate(-3.0f, 1.5f, 2.0f));

    Light* firefly1 = new Light(
        firefly1Transform,
        glm::vec3(1.0f, 0.9f, 0.3f),
        2.5f
    );
    scene->addLight(firefly1);

    TransformComposite* fireflyVisual1 = new TransformComposite();
    fireflyVisual1->addTransformation(new Translate(-3.0f, 1.5f, 2.0f));
    fireflyVisual1->addTransformation(new Scale(0.08f, 0.08f, 0.08f));
    scene->addObject(new DrawableObject(
        model3, fireflyVisual1, shaderProgramConstant, glm::vec3(1.0f, 0.9f, 0.3f)
    ));

    TransformComposite* firefly2Transform = new TransformComposite();
    firefly2Transform->addTransformation(new Translate(4.0f, 1.2f, -3.0f));

    Light* firefly2 = new Light(
        firefly2Transform,
        glm::vec3(1.0f, 1.0f, 0.4f),
        2.5f
    );
    scene->addLight(firefly2);

    TransformComposite* fireflyVisual2 = new TransformComposite();
    fireflyVisual2->addTransformation(new Translate(4.0f, 1.2f, -3.0f));
    fireflyVisual2->addTransformation(new Scale(0.08f, 0.08f, 0.08f));
    scene->addObject(new DrawableObject(
        model3, fireflyVisual2, shaderProgramConstant, glm::vec3(1.0f, 1.0f, 0.4f)
    ));

    TransformComposite* firefly3Transform = new TransformComposite();
    firefly3Transform->addTransformation(new Translate(0.0f, 2.0f, 0.0f));

    Light* firefly3 = new Light(
        firefly3Transform,
        glm::vec3(0.3f, 1.0f, 0.4f),
        2.5f
    );
    scene->addLight(firefly3);

    TransformComposite* fireflyVisual3 = new TransformComposite();
    fireflyVisual3->addTransformation(new Translate(0.0f, 2.0f, 0.0f));
    fireflyVisual3->addTransformation(new Scale(0.08f, 0.08f, 0.08f));
    scene->addObject(new DrawableObject(
        model3, fireflyVisual3, shaderProgramConstant, glm::vec3(0.3f, 1.0f, 0.4f)
    ));

    TransformComposite* firefly4Transform = new TransformComposite();
    firefly4Transform->addTransformation(new Translate(-2.0f, 1.8f, -6.0f));

    Light* firefly4 = new Light(
        firefly4Transform,
        glm::vec3(1.0f, 0.6f, 0.2f),
        2.5f
    );
    scene->addLight(firefly4);

    TransformComposite* fireflyVisual4 = new TransformComposite();
    fireflyVisual4->addTransformation(new Translate(-2.0f, 1.8f, -6.0f));
    fireflyVisual4->addTransformation(new Scale(0.08f, 0.08f, 0.08f));
    scene->addObject(new DrawableObject(
        model3, fireflyVisual4, shaderProgramConstant, glm::vec3(1.0f, 0.6f, 0.2f)
    ));

    printf("\n  NIGHT FOREST scene created:\n");
    printf("  - %d trees (Lambert) - dark\n", treeCount);
    printf("  - %d bushes (Phong) - dark\n", bushCount);
    printf("  - 3 benches (Blinn)\n");
    printf("  - GROUND + PATH: dark constant colors (no lighting needed)\n");
    printf("  - %d GLOWING fireflies: 2× YELLOW + 1× GREEN + 1× ORANGE\n", scene->getLightCount());
    printf("  - Low ambient (0.05) + High intensity (2.5) = DRAMATIC EFFECT!\n\n");

    return scene;
}