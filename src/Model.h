#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
// backup pred velkym commitom
class Model
{
private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    int vertexCount;

public:
    Model(float* vertices, int count);
    void draw();
};
#endif
