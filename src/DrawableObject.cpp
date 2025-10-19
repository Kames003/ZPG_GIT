#include "DrawableObject.h"
#include "Model.h"
#include "TransformComponent.h"
#include "ShaderProgram.h"

// Pôvodný konštruktor (bez farby)
DrawableObject::DrawableObject(Model* m, TransformComponent* t, ShaderProgram* sp)
    : model(m), transformation(t), shaderProgram(sp),
      objectColor(1.0f, 1.0f, 1.0f), hasColor(false)
{ }

// ← NOVÝ konštruktor (s farbou)
DrawableObject::DrawableObject(Model* m, TransformComponent* t, ShaderProgram* sp, glm::vec3 color)
    : model(m), transformation(t), shaderProgram(sp),
      objectColor(color), hasColor(true)
{ }

void DrawableObject::draw()
{
    shaderProgram->use();
    shaderProgram->setUniform("modelMatrix", transformation->getMatrix());

    // ← KRITICKÉ: Pošli farbu pri KAŽDOM draw()
    if (hasColor) {
        shaderProgram->setUniform("objectColor", objectColor);
    }

    model->draw();
}

void DrawableObject::setObjectColor(const glm::vec3& color)
{
    objectColor = color;
    hasColor = true;
}

Model* DrawableObject::getModel()
{
    return model;
}

TransformComponent* DrawableObject::getTransformation()
{
    return transformation;
}

ShaderProgram* DrawableObject::getShaderProgram()
{
    return shaderProgram;
}