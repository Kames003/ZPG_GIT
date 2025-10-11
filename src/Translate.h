#pragma once
#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "Transformation.h"

class Translate : public Transformation
{
public:
    Translate(float x, float y, float z);
};

#endif
