#include "Light.h"
#include "TransformComponent.h"


void Light::setColor(const glm::vec3& col)
{
    color = col;
    notifyAll();  // ← Učiteľ (24:41): "notify all"
}

void Light::setIntensity(float intens)
{
    intensity = intens;
    notifyAll();
}