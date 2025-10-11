#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>

class DrawableObject;

using namespace std;

class Scene
{
private:
    vector<DrawableObject*> objects;

public:
    Scene();

    void addObject(DrawableObject* object);
    void draw();
    void clear();
    vector<DrawableObject*>& getObjects() { return objects; }
};

#endif
