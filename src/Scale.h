#pragma once
#ifndef SCALE_H
#define SCALE_H

#include "TransformComponent.h"

// predstavuje elementárnu operáciu ( leaf )


class Scale : public TransformComponent
{
public:
    Scale(float x, float y, float z);
};

#endif
