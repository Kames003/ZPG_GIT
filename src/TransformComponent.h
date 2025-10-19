#pragma once // nechceme viacnásobný include
#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// abstraktná základná trieda -> z nej dedia všetky ostatné transformácie
class TransformComponent
{
protected:
    glm::mat4 matrix;

public:
    TransformComponent();
    virtual glm::mat4 getMatrix(); // spoločná operácia
};

#endif
