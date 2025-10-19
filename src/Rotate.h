#pragma once
#ifndef ROTATE_H
#define ROTATE_H

#include "TransformComponent.h"

// predstavuje elementárnu operáciu ( leaf )

class Rotate : public TransformComponent
{
public:
    Rotate(float angle, float x, float y, float z);
};

#endif
