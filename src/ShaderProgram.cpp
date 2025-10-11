#include "ShaderProgram.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>

ShaderProgram::ShaderProgram(const std::vector<Shader*>& shaders)
{
    // Vytvor shader program, priraď ID
    programID = glCreateProgram();

    // Attach všetky shadery pomocou ich metódy
    for (auto shader : shaders)
    {
        shader->attachShader(programID);  //  Volá Shader::attachShader()
    }

    // Link
    glLinkProgram(programID);

    // Kontrola linkovania
    GLint status;
    glGetProgramiv(programID, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(programID, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }
}

ShaderProgram::~ShaderProgram()
{
    if (programID != 0)
    {
        glDeleteProgram(programID);
    }
}


void ShaderProgram::use()
{
    glUseProgram(programID);
}



void ShaderProgram::setUniform(const char* name, int value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniform1i(location, value);
}

void ShaderProgram::setUniform(const char* name, float value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniform1f(location, value);
}

void ShaderProgram::setUniform(const char* name, const glm::vec3& value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char* name, const glm::mat4& value)
{
    GLint location = glGetUniformLocation(programID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

// Observer pattern - implementácia update metódy
void ShaderProgram::update(Camera* camera)
{
    // Automaticky aktualizuj view matrix, keď sa kamera zmení
    use();
    setUniform("viewMatrix", camera->getViewMatrix());
}