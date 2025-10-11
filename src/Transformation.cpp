#include "Transformation.h"

Transformation::Transformation()
{
    matrix = glm::mat4(1.0f);
    // konštruktor zaručí vytvorenie inštancie triedy s jednotkovou maticou
}

glm::mat4 Transformation::getMatrix()
{
    return matrix;
}
