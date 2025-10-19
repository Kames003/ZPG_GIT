#pragma once
#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "TransformComponent.h"

// predstavuje elementárnu operáciu ( leaf )

class Translate : public TransformComponent
{
public:
    Translate(float x, float y, float z);
    // getmatrix z TransformComponent je zdedená alebo overridnuta ( svoja implementácia pre daný leaf )
};

#endif
