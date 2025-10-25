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

// ========================================
// CONSTRUCTOR
// ========================================
SceneFactory::SceneFactory(
    Model* m1, Model* m2, Model* m3, Model* suziFlat, Model* suziSmooth,
    Model* tree, Model* bushes, Model* gift, Model* plain, Model* bench,
    ShaderProgram* sp1, ShaderProgram* sp2,
    ShaderProgram* spPhong, ShaderProgram* spLambert, 
    ShaderProgram* spConstant, ShaderProgram* spBlinn,
    ShaderProgram* spTree, ShaderProgram* spBush, 
    ShaderProgram* spGround, ShaderProgram* spPath, ShaderProgram* spBench)
    
    : model1(m1), model2(m2), model3(m3), 
      modelSuziFlat(suziFlat), modelSuziSmooth(suziSmooth),
      modelTree(tree), modelBushes(bushes), modelGift(gift), 
      modelPlain(plain), modelBench(bench),
      shaderProgram1(sp1), shaderProgram2(sp2),
      shaderProgramPhong(spPhong), shaderProgramLambert(spLambert),
      shaderProgramConstant(spConstant), shaderProgramBlinn(spBlinn),
      shaderProgramTree(spTree), shaderProgramBush(spBush),
      shaderProgramGround(spGround), shaderProgramPath(spPath), 
      shaderProgramBench(spBench)
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
    
    return scene;
}

// ========================================
// SCENE 2: SOLAR SYSTEM
// ========================================
Scene* SceneFactory::createSolarSystemScene() {
    printf("Creating Scene 2: Solar System\n");
    
    Scene* scene = new Scene();
    
    // SUN
    TransformComposite* sunTransform = new TransformComposite();
    sunTransform->addTransformation(new Translate(0.0f, 0.0f, 0.0f));
    sunTransform->addTransformation(new Scale(0.3f, 0.3f, 0.3f));
    DrawableObject* sun = new DrawableObject(
        model3, sunTransform, shaderProgramConstant, glm::vec3(1.0f, 0.9f, 0.0f)
    );
    scene->addObject(sun);
    
    // EARTH
    TransformComposite* earthOrbit = new TransformComposite();
    earthOrbit->addTransformation(new Rotate(0.0f, 0.0f, 1.0f, 0.0f));
    earthOrbit->addTransformation(new Translate(1.2f, 0.0f, 0.0f));
    earthOrbit->addTransformation(new Scale(0.15f, 0.15f, 0.15f));
    DrawableObject* earth = new DrawableObject(
        model3, earthOrbit, shaderProgramPhong, glm::vec3(0.2f, 0.4f, 0.8f)
    );
    scene->addObject(earth);
    
    // MOON
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
    
    printf("Solar System created\n");
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
// SCENE 5: LIGHTING DEMO (4 spheres)
// ========================================
Scene* SceneFactory::createLightingDemoScene() {
    printf("Creating Scene 5: Lighting Demo\n");
    
    Scene* scene = new Scene();
    glm::vec3 greenColor(0.2f, 0.8f, 0.2f);
    
    // Sphere 1 - Top
    TransformComposite* sphere1Transform = new TransformComposite();
    sphere1Transform->addTransformation(new Translate(0.0f, 0.6f, 0.0f));
    sphere1Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    DrawableObject* sphere1 = new DrawableObject(model3, sphere1Transform, shaderProgramPhong, greenColor);
    scene->addObject(sphere1);
    
    // Sphere 2 - Right
    TransformComposite* sphere2Transform = new TransformComposite();
    sphere2Transform->addTransformation(new Translate(0.6f, 0.0f, 0.0f));
    sphere2Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    DrawableObject* sphere2 = new DrawableObject(model3, sphere2Transform, shaderProgramPhong, greenColor);
    scene->addObject(sphere2);
    
    // Sphere 3 - Bottom
    TransformComposite* sphere3Transform = new TransformComposite();
    sphere3Transform->addTransformation(new Translate(0.0f, -0.6f, 0.0f));
    sphere3Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    DrawableObject* sphere3 = new DrawableObject(model3, sphere3Transform, shaderProgramPhong, greenColor);
    scene->addObject(sphere3);
    
    // Sphere 4 - Left
    TransformComposite* sphere4Transform = new TransformComposite();
    sphere4Transform->addTransformation(new Translate(-0.6f, 0.0f, 0.0f));
    sphere4Transform->addTransformation(new Scale(0.25f, 0.25f, 0.25f));
    DrawableObject* sphere4 = new DrawableObject(model3, sphere4Transform, shaderProgramPhong, greenColor);
    scene->addObject(sphere4);
    
    printf("4 spheres created\n");
    return scene;
}

// ========================================
// SCENE 6: COMPLEX SCENE (20+ objects)
// ========================================
Scene* SceneFactory::createComplexScene() {
    printf("Creating Scene 6: Complex Scene\n");
    
    Scene* scene = new Scene();
    
    // 4 Suzi Flat - corners
    float suziPositions[][2] = {
        {-0.7f, 0.7f}, {0.7f, 0.7f}, {-0.7f, -0.7f}, {0.7f, -0.7f}
    };
    for (int i = 0; i < 4; i++) {
        TransformComposite* suziTransform = new TransformComposite();
        suziTransform->addTransformation(new Translate(suziPositions[i][0], suziPositions[i][1], 0.0f));
        suziTransform->addTransformation(new Scale(0.12f, 0.12f, 0.12f));
        scene->addObject(new DrawableObject(modelSuziFlat, suziTransform, shaderProgram1));
    }
    
    // 5 Trees - top row
    for (int i = 0; i < 5; i++) {
        TransformComposite* treeTransform = new TransformComposite();
        treeTransform->addTransformation(new Translate(-0.6f + i * 0.3f, 0.35f, 0.0f));
        treeTransform->addTransformation(new Scale(0.03f, 0.03f, 0.03f));
        scene->addObject(new DrawableObject(modelTree, treeTransform, shaderProgram2));
    }
    
    // 5 Bushes - bottom row
    for (int i = 0; i < 5; i++) {
        TransformComposite* bushTransform = new TransformComposite();
        bushTransform->addTransformation(new Translate(-0.5f + i * 0.25f, -0.35f, 0.0f));
        bushTransform->addTransformation(new Scale(0.15f, 0.15f, 0.15f));
        scene->addObject(new DrawableObject(modelBushes, bushTransform, shaderProgram1));
    }
    
    // 4 Spheres - edges
    float spherePositions[][2] = {
        {0.9f, 0.0f}, {-0.9f, 0.0f}, {0.0f, 0.9f}, {0.0f, -0.9f}
    };
    for (int i = 0; i < 4; i++) {
        TransformComposite* sphereTransform = new TransformComposite();
        sphereTransform->addTransformation(new Translate(spherePositions[i][0], spherePositions[i][1], 0.0f));
        sphereTransform->addTransformation(new Scale(0.07f, 0.07f, 0.07f));
        scene->addObject(new DrawableObject(model3, sphereTransform, shaderProgram2));
    }
    
    // 4 Gifts - center
    float giftPositions[][2] = {
        {-0.15f, 0.1f}, {0.15f, 0.1f}, {-0.15f, -0.1f}, {0.15f, -0.1f}
    };
    for (int i = 0; i < 4; i++) {
        TransformComposite* giftTransform = new TransformComposite();
        giftTransform->addTransformation(new Translate(giftPositions[i][0], giftPositions[i][1], 0.0f));
        giftTransform->addTransformation(new Scale(0.18f, 0.18f, 0.18f));
        scene->addObject(new DrawableObject(modelGift, giftTransform, shaderProgram1));
    }
    
    printf("Complex scene created: 22 objects\n");
    return scene;
}

// ========================================
// SCENE 7: FOREST
// ========================================
Scene* SceneFactory::createForestScene() {
    printf("Creating Scene 7: Forest\n");
    
    Scene* scene = new Scene();
    
    // Ground
    TransformComposite* groundTransform = new TransformComposite();
    groundTransform->addTransformation(new Translate(0.0f, 0.0f, 0.0f));
    groundTransform->addTransformation(new Scale(20.0f, 1.0f, 20.0f));
    scene->addObject(new DrawableObject(modelPlain, groundTransform, shaderProgramGround));
    
    // Path
    TransformComposite* pathTransform = new TransformComposite();
    pathTransform->addTransformation(new Translate(0.0f, 0.01f, 0.0f));
    pathTransform->addTransformation(new Scale(2.0f, 1.0f, 20.0f));
    scene->addObject(new DrawableObject(modelPlain, pathTransform, shaderProgramPath));
    
    // Benches (3x)
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
            scene->addObject(new DrawableObject(
                modelTree, treeTransform, shaderProgramLambert, glm::vec3(0.11f, 0.38f, 0.0f)
            ));
            treeCount++;
        }
    }
    
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
            scene->addObject(new DrawableObject(
                modelBushes, bushTransform, shaderProgramPhong, glm::vec3(0.2f, 0.8f, 0.2f)
            ));
            bushCount++;
        }
    }
    
    printf("Forest: %d trees, %d bushes, 3 benches\n", treeCount, bushCount);
    return scene;
}