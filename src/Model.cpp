#include "Model.h"
#include <cstddef>

Model::Model(float* vertices, int count)
{
    vertexCount = count;

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, count * 6 * sizeof(float), vertices, GL_STATIC_DRAW); // 6 floats!

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // attribute 0: position (x, y, z)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);

    // attribute 1: normal (nx, ny, nz)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
}

void Model::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
