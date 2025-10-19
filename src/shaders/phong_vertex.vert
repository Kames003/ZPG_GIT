#version 330

layout(location=0) in vec3 vp; // pozícia vertexu
layout(location=1) in vec3 vn; // normála vertexu

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 worldPosition;
out vec3 worldNormal;

void main(void)
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);

    // Transformácia pozície
    worldPosition = modelMatrix * vec4(vp, 1.0);

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    worldNormal = normalize(normalMatrix * vn);
}