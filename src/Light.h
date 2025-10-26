#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <vector>
#include "Subject.h"
#include "TransformComponent.h"  // ← KRITICKÉ! Plná definícia pre inline metódy

/**
 * Svetlo s podporou transformácií (kompozícia ako DrawableObject)
 *
 * Učiteľ (11:36): "Každá scéna sa bude stávať o svoje svetla"
 * Učiteľ (21:09): "když v transformácii použijete transform component a transform composite"
 * Učiteľ (7:23): "které to svetlo v tom poli mám, na ktoré pozícii"
 */
class Light : public Subject
{
private:
    TransformComponent* transformation;  // ← Učiteľ (21:09): "transform component"
    glm::vec3 color;
    float intensity;
    int shaderIndex;  // ← Učiteľ (7:23): "na ktoré pozícii v tom poli"

public:
    /**
     * Constructor s transformáciou
     *
     * Učiteľ (21:09): "když v transformácii použijete transform component"
     */
    Light(TransformComponent* transform,
          glm::vec3 col = glm::vec3(1.0f, 1.0f, 1.0f),
          float intens = 1.0f)
        : transformation(transform), color(col), intensity(intens), shaderIndex(-1)
    { }

    /**
     * Destructor - zmaže transformáciu
     */
    ~Light() {
        if (transformation) {
            delete transformation;
        }
    }

    // ========================================
    // GETTERS
    // ========================================

    /**
     * Pozícia svetla z transformačnej matice
     *
     * Extrahovanie pozície z matice 4x4:
     * mat[3] = translation (x, y, z, w)
     */
    glm::vec3 getPosition() const {
        if (transformation) {
            glm::mat4 mat = transformation->getMatrix();  // ← Teraz funguje!
            return glm::vec3(mat[3]);  // Pozícia z matice (4. stĺpec)
        }
        return glm::vec3(0.0f);
    }

    glm::vec3 getColor() const { return color; }
    float getIntensity() const { return intensity; }

    /**
     * Učiteľ (7:23): "které to svetlo v tom poli mám, na ktoré pozícii"
     */
    int getShaderIndex() const { return shaderIndex; }

    TransformComponent* getTransformation() const { return transformation; }

    // ========================================
    // SETTERS
    // ========================================

    /**
     * Nastaví novú transformáciu (pre animáciu)
     *
     * Učiteľ (27:10): "Observer sa musí rozhodnúť, čo by delať"
     * → Observer dostane notifikáciu a sám si vytiahne novú pozíciu
     */
    void setTransformation(TransformComponent* newTransform) {
        if (transformation) {
            delete transformation;  // ← Teraz funguje!
        }
        transformation = newTransform;
        notifyAll();  // ← Observer pattern!
    }

    void setColor(const glm::vec3& col);
    void setIntensity(float intens);

    /**
     * Učiteľ (7:33): "Takže vy si budete musieť pamatovať to svetlo,
     *                 na ktorú pozícii v tom poli patrím"
     */
    void setShaderIndex(int index) {
        shaderIndex = index;
    }
};

#endif