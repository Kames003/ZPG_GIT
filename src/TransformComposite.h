#pragma once
#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "TransformComponent.h"
#include <vector>

using namespace std;

class TransformComposite : public TransformComponent // je tiež Component
{
private:
    vector<TransformComponent*> transformations;
    // môžem držať čokoľvek čo je component - translate, rotate, scale, transformcompostie
    // 3x leaf | vnoreny composite

public:
    TransformComposite();

    void addTransformation(TransformComponent* transformation);
    glm::mat4 getMatrix() override; // rekurzívna
    void clear() { transformations.clear(); }
};

#endif
