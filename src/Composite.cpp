#include "Composite.h"

Composite::Composite()
{
    matrix = glm::mat4(1.0f);
}

void Composite::addTransformation(Transformation* transformation)
{
    transformations.push_back(transformation);
}
// rekurzivna implementácia
glm::mat4 Composite::getMatrix()
{
    glm::mat4 result = glm::mat4(1.0f);

    for (Transformation* t : transformations)
    {
        result = result * t->getMatrix(); // rekurzívne volanie pre composite
    }

    return result;
}
