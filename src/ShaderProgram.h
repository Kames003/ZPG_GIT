#pragma once
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "ICameraObserver.h"
#include "ILightObserver.h"

class Shader;
class Camera;  // Forward declaration

// čo by tu mohlo byť nakolko sa jedna o c++
// class ShaderProgram : public ICameraObserver, public ILightObserver



class ShaderProgram : public ICameraObserver, public ILightObserver
{
private:
    GLuint programID;

public:
    ShaderProgram(const std::vector<Shader*>& shaders); // berie shaders
    ~ShaderProgram();

    void use();

    // Valid C++ preťaženie metódy pre posielanie uniformných premenných
    void setUniform(const char* name, int value);
    void setUniform(const char* name, float value);
    void setUniform(const char* name, const glm::vec3& value);
    void setUniform(const char* name, const glm::mat4& value);

    // Observer pattern
    void update(Camera* camera) override; // implementuje interface
    void onLightUpdate(Light* light) override;
};

#endif