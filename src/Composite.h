#pragma once
#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "Transformation.h"
#include <vector>

using namespace std;

class Composite : public Transformation
{
private:
    vector<Transformation*> transformations;

public:
    Composite();

    void addTransformation(Transformation* transformation);
    glm::mat4 getMatrix() override;
    void clear() { transformations.clear(); }
};

#endif
