#pragma once
#ifndef SCALE_H
#define SCALE_H

#include "Transformation.h"

class Scale : public Transformation
{
public:
    Scale(float x, float y, float z);
};

#endif
