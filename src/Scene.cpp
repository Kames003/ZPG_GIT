#include "Scene.h"
#include "DrawableObject.h"

Scene::Scene() { }

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
}
