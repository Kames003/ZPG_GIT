#include "Translate.h"

Translate::Translate(float x, float y, float z)
{
    matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}
