#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <vector>

#include "Observer.h"

// Forward declaration
#include "Subject.h"

class Light : public Subject
// no need for own observerCollection
{
private:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

public:
    Light(glm::vec3 pos = glm::vec3(10.0f, 10.0f, 10.0f),
          glm::vec3 col = glm::vec3(1.0f, 1.0f, 1.0f),
          float intens = 1.0f);

    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getColor() const { return color; }
    float getIntensity() const { return intensity; }

    // Setters (s notifikáciou)
    void setPosition(const glm::vec3& pos);
    void setColor(const glm::vec3& col);
    void setIntensity(float intens);
};

#endif