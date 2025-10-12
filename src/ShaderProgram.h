#pragma once
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "ICameraObserver.h"

class Shader;
class Camera;  // Forward declaration

class ShaderProgram : public ICameraObserver
{
private:
    GLuint programID;

public:
    ShaderProgram(const std::vector<Shader*>& shaders);
    ~ShaderProgram();

    void use();

    // Valid C++ preťaženie metódy pre posielanie uniformných premenných
    void setUniform(const char* name, int value);
    void setUniform(const char* name, float value);
    void setUniform(const char* name, const glm::vec3& value);
    void setUniform(const char* name, const glm::mat4& value);

    // Observer pattern
    void update(Camera* camera) override;
};

#endif