#include "ShaderLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

std::string FileShaderLoader::loadShaderCode()
{
    std::ifstream file(filePath); // otvor
    if (!file.is_open()) // existuješ ?
    {
        std::cerr << "ERROR: Unable to open shader file: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::stringstream buffer; // načítaj celý obsah súboru do stringu
    buffer << file.rdbuf(); // všetmk  araz
    file.close();
    
    std::cout << "Shader načítaný z filu: " << filePath << std::endl;
    return buffer.str(); // vrat string s glsl kódom
}