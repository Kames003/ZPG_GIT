#include "Light.h"
#include "ILightObserver.h"
#include <algorithm>

Light::Light(glm::vec3 pos, glm::vec3 col, float intens)
    : position(pos), color(col), intensity(intens)
{
}

void Light::attachObserver(ILightObserver* observer)
{
    observers.push_back(observer);
}

void Light::detachObserver(ILightObserver* observer)
{
    observers.erase(
        std::remove(observers.begin(), observers.end(), observer),
        observers.end()
    );
}

void Light::notifyObservers()
{
    for (ILightObserver* observer : observers)
    {
        observer->onLightUpdate(this);
    }
}

void Light::setPosition(const glm::vec3& pos)
{
    position = pos;
    notifyObservers();
}

void Light::setColor(const glm::vec3& col)
{
    color = col;
    notifyObservers();
}

void Light::setIntensity(float intens)
{
    intensity = intens;
    notifyObservers();
}