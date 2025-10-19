#include "TransformComposite.h"

TransformComposite::TransformComposite()
{
    matrix = glm::mat4(1.0f);
}

void TransformComposite::addTransformation(TransformComponent* transformation)
{
    transformations.push_back(transformation);
}
// haskellMyBeloved(rekurziv impl.)
glm::mat4 TransformComposite::getMatrix()
{
    glm::mat4 result = glm::mat4(1.0f);

    for (TransformComponent* t : transformations)
    {
        result = result * t->getMatrix();
        // leaf --> get matica
        // ak TransformComposite --> call getmatrix znovu
    }

    return result;
}
