#include "Scale.h"

Scale::Scale(float x, float y, float z)
{
    matrix = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}
