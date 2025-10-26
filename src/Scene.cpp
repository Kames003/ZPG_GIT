#include "Scene.h"
#include "DrawableObject.h"
#include "Light.h"
#include <algorithm>

// ========================================
// CONSTRUCTOR
// ========================================
Scene::Scene()
{
    // Inicializácia - vektory sa inicializujú automaticky
}

// ========================================
// OBJEKTY
// ========================================
void Scene::addObject(DrawableObject* object)
{
    objects.push_back(object);
}

void Scene::draw()
{
    for (DrawableObject* obj : objects)
    {
        obj->draw();
    }
}

void Scene::clear()
{
    objects.clear();
    lights.clear();
}

std::vector<DrawableObject*>& Scene::getObjects()
{
    return objects;
}

// ========================================
// SVETLÁ
// ========================================

/**
 * Pridá svetlo do scény a automaticky mu pridelí index
 *
 * Učiteľ (7:26): "Ja potrebujem si pamatovať,
 * které to svetlo v tom poli mám, na ktoré pozícii"
 */
void Scene::addLight(Light* light)
{
    if (!light) return;

    // Pridelíme svetlu index podľa pozície v poli
    int index = static_cast<int>(lights.size());
    light->setShaderIndex(index);

    lights.push_back(light);
}

/**
 * Odstráni svetlo a prepočíta indexy zvyšných svetiel
 */
void Scene::removeLight(Light* light)
{
    auto it = std::find(lights.begin(), lights.end(), light);
    if (it != lights.end())
    {
        lights.erase(it);

        // Prepočítaj indexy pre zvyšné svetlá
        for (size_t i = 0; i < lights.size(); i++)
        {
            lights[i]->setShaderIndex(static_cast<int>(i));
        }
    }
}

std::vector<Light*>& Scene::getLights()
{
    return lights;
}

int Scene::getLightCount() const
{
    return static_cast<int>(lights.size());
}