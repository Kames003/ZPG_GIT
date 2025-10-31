#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>

class DrawableObject;
class Light;

class Scene
{
private:
    std::vector<DrawableObject*> objects;
    std::vector<Light*> lights;

public:
    Scene();

    // ========================================
    // OBJEKTY
    // ========================================
    void addObject(DrawableObject* object);
    void draw();
    void clear();
    std::vector<DrawableObject*>& getObjects();

    // ========================================
    // SVETLÁ
    // ========================================
    void addLight(Light* light);
    void removeLight(Light* light);
    std::vector<Light*>& getLights();
    int getLightCount() const;
};

#endif