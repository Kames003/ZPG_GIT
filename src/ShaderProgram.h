#pragma once
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Observer.h"

// Forward declaration
class Shader;
class Subject;
class Camera;
class Light;


class ShaderProgram : public Observer
{
private:
    GLuint programID;
    void updateCameraUniforms(Camera* camera);
    void updateLightUniforms(Light* light);

public:
    ShaderProgram(const std::vector<Shader*>& shaders); // berie shaders
    ~ShaderProgram();

    void use();

    // Valid C++ preťaženie metódy pre posielanie uniformných premenných
    void setUniform(const char* name, int value);
    void setUniform(const char* name, float value);
    void setUniform(const char* name, const glm::vec3& value);
    void setUniform(const char* name, const glm::mat4& value);
    void setUniform(const char* name, const glm::vec4& value);

    // Observer interface
    void notify(Subject* subject) override;

    void setLights(const std::vector<Light*>& lights);
    void setLight(int index, Light* light);

};

#endif