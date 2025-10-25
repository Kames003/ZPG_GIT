#pragma once
#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>

// strategy pattern prosto a len volame načitaj a on nemusí vedieť ako
// implementácia strategy patternu
class IShaderLoader
{
public:
    virtual ~IShaderLoader() = default;
    virtual std::string loadShaderCode() = 0;
};

// inline kod
class InlineShaderLoader : public IShaderLoader
{
private:
    std::string shaderCode;
    
public:
    explicit InlineShaderLoader(const char* code) : shaderCode(code) {}
    
    std::string loadShaderCode() override {
        return shaderCode;
    }
};

// súbor
class FileShaderLoader : public IShaderLoader
{
private:
    std::string filePath;
    
public:
    explicit FileShaderLoader(const char* path) : filePath(path) {}
    
    std::string loadShaderCode() override;
};

#endif