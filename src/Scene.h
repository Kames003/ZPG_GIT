#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>

class DrawableObject;
class Light;

/**
 * Scéna = kolekcia objektov + svetiel
 *
 * KOMPOZÍCIA: Každá scéna má svoje svetlá (ako odporúčal učiteľ)
 * - Svetlá sa NEZDIEĽAJÚ medzi scénami
 * - Každá scéna si spravuje svoje osvetlenie
 * - Automaticky priraďuje indexy svetlám
 */
class Scene
{
private:
    std::vector<DrawableObject*> objects;
    std::vector<Light*> lights;

public:
    Scene();



    // ========================================
    // OBJEKTY
    // ========================================
    void addObject(DrawableObject* object);
    void draw();
    void clear();
    std::vector<DrawableObject*>& getObjects();

    // ========================================
    // SVETLÁ
    // ========================================
    void addLight(Light* light);
    void removeLight(Light* light);
    std::vector<Light*>& getLights();
    int getLightCount() const;
};

#endif