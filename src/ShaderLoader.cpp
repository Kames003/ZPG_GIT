#include "ShaderLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

std::string FileShaderLoader::loadShaderCode()
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "ERROR: Unable to open shader file: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    std::cout << "✓ Shader loaded from file: " << filePath << std::endl;
    return buffer.str();
}