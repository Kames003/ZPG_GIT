#include "Light.h"
#include "Light.h"
#include <algorithm>

Light::Light(glm::vec3 pos, glm::vec3 col, float intens)
    : position(pos), color(col), intensity(intens)
{
}

void Light::setPosition(const glm::vec3& pos)
{
    position = pos;
    notifyAll();
}

void Light::setColor(const glm::vec3& col)
{
    color = col;
    notifyAll();
}

void Light::setIntensity(float intens)
{
    intensity = intens;
    notifyAll();
}