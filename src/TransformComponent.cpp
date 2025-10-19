#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
    matrix = glm::mat4(1.0f);
    // konštruktor zaručí vytvorenie inštancie triedy s jednotkovou maticou
}

glm::mat4 TransformComponent::getMatrix()
{
    return matrix;
}
