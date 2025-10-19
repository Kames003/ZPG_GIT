#pragma once
#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <string>

// ============================================
// STRATEGY INTERFACE - definuje zmluvu
// ============================================
class IShaderLoader
{
public:
    virtual ~IShaderLoader() = default;
    virtual std::string loadShaderCode() = 0;
};

// ============================================
// CONCRETE STRATEGY 1 - Inline kód
// ============================================
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

// ============================================
// CONCRETE STRATEGY 2 - Súbor
// ============================================
class FileShaderLoader : public IShaderLoader
{
private:
    std::string filePath;
    
public:
    explicit FileShaderLoader(const char* path) : filePath(path) {}
    
    std::string loadShaderCode() override;
};

#endif