#pragma once
#ifndef SCENE_FACTORY_H
#define SCENE_FACTORY_H

class Scene;
class Model;
class ShaderProgram;

class SceneFactory {
private:
    // MODELY
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

    // SHADER PROGRAMY
    ShaderProgram* shaderProgram1;
    ShaderProgram* shaderProgram2;
    ShaderProgram* shaderProgramPhong;
    ShaderProgram* shaderProgramLambert;
    ShaderProgram* shaderProgramConstant;
    ShaderProgram* shaderProgramBlinn;

    // Len pre Ground/Path (BEZ osvetlenia)
    ShaderProgram* shaderProgramGround;
    ShaderProgram* shaderProgramPath;

    // BOD 3b: Backface test shadery
    ShaderProgram* shaderProgramPhongWrong;
    ShaderProgram* shaderProgramPhongCorrect;

public:
    SceneFactory(
        Model* m1, Model* m2, Model* m3, Model* suziFlat, Model* suziSmooth,
        Model* tree, Model* bushes, Model* gift, Model* plain, Model* bench,
        ShaderProgram* sp1, ShaderProgram* sp2,
        ShaderProgram* spPhong, ShaderProgram* spLambert,
        ShaderProgram* spConstant, ShaderProgram* spBlinn,
        ShaderProgram* spGround, ShaderProgram* spPath,
        ShaderProgram* spPhongWrong, ShaderProgram* spPhongCorrect
    );

    Scene* createPhongTestScene();
    Scene* createSolarSystemScene();
    Scene* createBothObjectsScene();
    Scene* createRotatingTriangleScene();
    Scene* createLightingDemoScene();
    Scene* createForestScene();
    Scene* createBackfaceLightingTest();
};

#endif