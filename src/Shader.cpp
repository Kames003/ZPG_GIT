#include "Shader.h"
#include <stdio.h>
#include <stdlib.h>
#include "ShaderLoader.h"

Shader::Shader(GLenum type, IShaderLoader* loader) // constructor obsahujúci aký je to shader a id skompilovaného shadera
{
    shaderType = type;
    shaderID = glCreateShader(type); // OpenGL vygeneruje ID


    // nevie či je to FileShaderLoader alebo InlineShaderLoader
    // ale vie že ma funkciu loadShaderCode()
    std::string shaderCodeStr = loader->loadShaderCode();

    const char* shaderCode = shaderCodeStr.c_str();

    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);

    // Kontrola kompilace
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status); // daj kod do shadru
    if (status == GL_FALSE) // skompiluj shader
    {
        GLint infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, strInfoLog);

        const char* strShaderType = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;
    }
}


Shader::~Shader()
{
    if (shaderID != 0)
    {
        glDeleteShader(shaderID); // uvolni gpu pamäť
    }
}

void Shader::attachShader(GLuint programID) const

{
    glAttachShader(programID, shaderID);
    // pripoj shaderID k tomuto programu
}

GLenum Shader::getType() const
{
    return shaderType;
}