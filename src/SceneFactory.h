#pragma once
#ifndef SCENE_FACTORY_H
#define SCENE_FACTORY_H

// Forward declarations
class Scene;
class Model;
class ShaderProgram;

/**
 * Factory Method pattern pre tvorbu scén.
 * Zapuzdruje logiku tvorby scén (oddelenie od Application).
 */
class SceneFactory {
private:
    // ========================================
    // MODELY (referencie, Application ich vlastní)
    // ========================================
    Model* model1;           // Triangle
    Model* model2;           // Square
    Model* model3;           // Sphere
    Model* modelSuziFlat;
    Model* modelSuziSmooth;
    Model* modelTree;
    Model* modelBushes;
    Model* modelGift;
    Model* modelPlain;
    Model* modelBench;
    
    // ========================================
    // SHADER PROGRAMY (referencie, Application ich vlastní)
    // ========================================
    ShaderProgram* shaderProgram1;      // Red
    ShaderProgram* shaderProgram2;      // Green
    ShaderProgram* shaderProgramPhong;
    ShaderProgram* shaderProgramLambert;
    ShaderProgram* shaderProgramConstant;
    ShaderProgram* shaderProgramBlinn;
    ShaderProgram* shaderProgramTree;
    ShaderProgram* shaderProgramBush;
    ShaderProgram* shaderProgramGround;
    ShaderProgram* shaderProgramPath;
    ShaderProgram* shaderProgramBench;

public:
    /**
     * Constructor - dostane referencie na všetky zdieľané zdroje.
     */
    SceneFactory(
        Model* m1, Model* m2, Model* m3, Model* suziFlat, Model* suziSmooth,
        Model* tree, Model* bushes, Model* gift, Model* plain, Model* bench,
        ShaderProgram* sp1, ShaderProgram* sp2,
        ShaderProgram* spPhong, ShaderProgram* spLambert, 
        ShaderProgram* spConstant, ShaderProgram* spBlinn,
        ShaderProgram* spTree, ShaderProgram* spBush, 
        ShaderProgram* spGround, ShaderProgram* spPath, ShaderProgram* spBench
    );
    
    /**
     * Factory methods - vracajú nové Scene*.
     */
    Scene* createPhongTestScene();
    Scene* createSolarSystemScene();
    Scene* createBothObjectsScene();
    Scene* createRotatingTriangleScene();
    Scene* createLightingDemoScene();
    Scene* createComplexScene();
    Scene* createForestScene();
};

#endif