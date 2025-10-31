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


void Scene::addLight(Light* light)
{
    if (!light) return;


    int index = static_cast<int>(lights.size()); // pridelíme svetlu index podla pozicie v poli
    light->setShaderIndex(index);

    lights.push_back(light);
}

void Scene::removeLight(Light* light)
{
    auto it = std::find(lights.begin(), lights.end(), light);
    if (it != lights.end())
    {
        lights.erase(it);

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