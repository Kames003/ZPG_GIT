#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

// shader = jednotlivý shader ( vertex alebo fragment )

class IShaderLoader;

class Shader
{
private:
    GLuint shaderID; // privátne bez gettera
    GLenum shaderType;


public:
    Shader(GLenum type, IShaderLoader* loader);
    ~Shader();

    void attachShader(GLuint programID) const; // wrapper

    GLenum getType() const; // getter def., len typ, nie ID

};

#endif
