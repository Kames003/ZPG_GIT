#pragma once // nechceme viacnásobný include
#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transformation
{
protected:
    glm::mat4 matrix;

public:
    Transformation();
    virtual glm::mat4 getMatrix(); // spoločná operácia// / component + virtuálna metóda
};

#endif
