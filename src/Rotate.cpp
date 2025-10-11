#include "Rotate.h"

Rotate::Rotate(float angle, float x, float y, float z)
{
    matrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z));
}
