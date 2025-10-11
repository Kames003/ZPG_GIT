#pragma once
#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

class Model;
class Transformation;
class ShaderProgram;

class DrawableObject
{
private:
    Model* model;
    Transformation* transformation;
    ShaderProgram* shaderProgram;

public:
    DrawableObject(Model* m, Transformation* t, ShaderProgram* sp);

    void draw();

    // getters
    Model* getModel();
    Transformation* getTransformation();
    ShaderProgram* getShaderProgram();
};

#endif
