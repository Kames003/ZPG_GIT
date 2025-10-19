#version 330

in vec4 worldPosition;
in vec3 worldNormal;

out vec4 out_Color;

uniform vec3 lightPosition;
uniform vec3 objectColor;

void main(void)
{
    // AMBIENT
    vec3 ambient = 0.3 * objectColor;

    // DIFFUSE (Lambert)
    vec3 norm = normalize(worldNormal);
    vec3 lightDir = normalize(lightPosition - worldPosition.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * objectColor;

    // ← ATTENUATION
    float distance = length(lightPosition - worldPosition.xyz);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    // FINAL
    vec3 result = ambient + attenuation * diffuse;
    out_Color = vec4(result, 1.0);
}