#version 330

layout(location=0) in vec3 vp;
layout(location=1) in vec3 vn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 worldPosition;
out vec3 worldNormal;

void main(void)
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);

    // World space pozice
    worldPosition = modelMatrix * vec4(vp, 1.0);

    // OPRAVA: Správná normálová transformace
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    worldNormal = normalize(normalMatrix * vn);
}