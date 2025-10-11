#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

class Shader
{
private:
    GLuint shaderID; // privátne bez gettera
    GLenum shaderType;


public:
    Shader(const char* shaderCode, GLenum type);
    ~Shader();

    void attachShader(GLuint programID) const; // wrapper

    GLenum getType() const; // getter def., len typ, nie ID

};

#endif
