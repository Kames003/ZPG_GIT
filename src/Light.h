#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <vector>

// Forward declaration
class ILightObserver;

class Light
{
private:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

    std::vector<ILightObserver*> observers;

public:
    Light(glm::vec3 pos = glm::vec3(10.0f, 10.0f, 10.0f),
          glm::vec3 col = glm::vec3(1.0f, 1.0f, 1.0f),
          float intens = 1.0f);

    // Observer pattern
    void attachObserver(ILightObserver* observer);
    void detachObserver(ILightObserver* observer);
    void notifyObservers();

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