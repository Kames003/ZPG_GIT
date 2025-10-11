#include "DrawableObject.h"
#include "Model.h"
#include "Transformation.h"
#include "ShaderProgram.h"

DrawableObject::DrawableObject(Model* m, Transformation* t, ShaderProgram* sp)
    : model(m), transformation(t), shaderProgram(sp) { }

void DrawableObject::draw()
{
    shaderProgram->use(); // shader activation
    shaderProgram->setUniform("modelMatrix", transformation->getMatrix()); // send transformation matrix to shader
    model->draw();
}

Model* DrawableObject::getModel()
{
    return model;
}

Transformation* DrawableObject::getTransformation()
{
    return transformation;
}

ShaderProgram* DrawableObject::getShaderProgram()
{
    return shaderProgram;
}
