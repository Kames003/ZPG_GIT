#include "Light.h"
#include "TransformComponent.h"

/**
 * Učiteľ (27:10): "Observer sa musí rozhodnúť, čo by delať"
 * → Settery len notifikujú, Observer si sám vytiahne dáta (PULL prístup)
 */

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