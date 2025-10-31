#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <vector>
#include "Subject.h"
#include "TransformComponent.h"


class Light : public Subject
{
private:
    TransformComponent* transformation;
    glm::vec3 color;
    float intensity;
    int shaderIndex;

public:

    Light(TransformComponent* transform,
          glm::vec3 col = glm::vec3(1.0f, 1.0f, 1.0f),
          float intens = 1.0f)
        : transformation(transform), color(col), intensity(intens), shaderIndex(-1)
    { }

    /*
     transformation = odkaz na transformačny objekt ( pozícia )
     index nie je ešte pridelený ten pridelí scéna
     **/

    ~Light() {
        if (transformation) {
            delete transformation;
        }
    }

    glm::vec3 getPosition() const {
        if (transformation) {
            glm::mat4 mat = transformation->getMatrix(); // získa transformačnú maticu
            return glm::vec3(mat[3]); // pozicia z matice štvrtý stlpec
        }
        return glm::vec3(0.0f);
    }

    glm::vec3 getColor() const { return color; }
    float getIntensity() const { return intensity; }
    int getShaderIndex() const { return shaderIndex; }

    TransformComponent* getTransformation() const { return transformation; }


    void setTransformation(TransformComponent* newTransform) {
        if (transformation) {
            delete transformation;
        }
        transformation = newTransform;
        notifyAll();
    }

    void setColor(const glm::vec3& col);
    void setIntensity(float intens);


    void setShaderIndex(int index) {
        shaderIndex = index;
    }
};

#endif