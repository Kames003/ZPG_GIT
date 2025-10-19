#version 330

in vec4 worldPosition;
in vec3 worldNormal;

out vec4 out_Color;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 objectColor;

void main(void)
{
    // AMBIENT
    vec3 ambient = 0.25 * objectColor;

    // DIFFUSE
    vec3 norm = normalize(worldNormal);
    vec3 lightDir = normalize(lightPosition - worldPosition.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * objectColor;

    // SPECULAR (Blinn-Phong)
    vec3 viewDir = normalize(viewPosition - worldPosition.xyz);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 48.0);
    vec3 specular = 1.2 * spec * vec3(1.0);

    // ← ATTENUATION
    float distance = length(lightPosition - worldPosition.xyz);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    // FINAL
    vec3 result = ambient + attenuation * (diffuse + specular);
    out_Color = vec4(result, 1.0);
}