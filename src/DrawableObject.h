#pragma once
#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include <glm/glm.hpp>  // ← MUSÍ BYŤ!

class Model;
class TransformComponent;
class ShaderProgram;

class DrawableObject
{
private:
    Model* model;
    TransformComponent* transformation;
    ShaderProgram* shaderProgram;

    // ← NOVÉ: Farba objektu
    glm::vec3 objectColor;
    bool hasColor;

public:
    // konštruktor (bez farby)
    DrawableObject(Model* m, TransformComponent* t, ShaderProgram* sp);

    // Konštruktor (s farbou)
    DrawableObject(Model* m, TransformComponent* t, ShaderProgram* sp, glm::vec3 color);

    void draw();

    // Getters
    Model* getModel();
    TransformComponent* getTransformation();
    ShaderProgram* getShaderProgram();

    // ← NOVÝ: Setter pre farbu
    void setObjectColor(const glm::vec3& color);

    void setShaderProgram(ShaderProgram* sp) {
        shaderProgram = sp;
    }
};

#endif