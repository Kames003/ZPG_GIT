#pragma once
#ifndef ROTATE_H
#define ROTATE_H

#include "Transformation.h"

class Rotate : public Transformation
{
public:
    Rotate(float angle, float x, float y, float z);
};

#endif
