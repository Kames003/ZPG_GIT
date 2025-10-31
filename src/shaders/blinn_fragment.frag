#version 330

// ========================================
// INPUT
// ========================================
in vec4 worldPosition;
in vec3 worldNormal;

// ========================================
// OUTPUT
// ========================================
out vec4 out_Color;

// ========================================
// LIGHT STRUCT
// ========================================
#define MAX_LIGHTS 8

struct Light {
    vec4 position;
    vec3 ambient;    // Používa sa LEN pre prvé svetlo
    vec3 diffuse;
    vec3 specular;
    float intensity;
};

// ========================================
// UNIFORMS
// ========================================
uniform vec3 viewPosition;
uniform vec3 objectColor;
uniform Light lights[MAX_LIGHTS];
uniform int numberOfLights;

// ========================================
// MAIN - BLINN-PHONG SHADING
// ========================================
void main(void)
{
    // GLOBÁLNE AMBIENT - iba raz, mimo cyklu
    vec3 globalAmbient = objectColor * 0.15;

    //  AMBIENT z prvého svetla - LEN RAZ!
    vec3 ambient = vec3(0.0);
    if (numberOfLights > 0) {
        ambient = lights[0].ambient * objectColor * 0.1;
    }

    // Akumulácia diffuse a specular
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    // Cyklus cez všetky svetlá - LEN diffuse + specular!
    for (int i = 0; i < numberOfLights; i++) {
        Light light = lights[i];

        // DIFFUSE
        vec3 norm = normalize(worldNormal);
        vec3 lightDir = normalize(light.position.xyz - worldPosition.xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * objectColor * light.diffuse * light.intensity;

        // SPECULAR (Blinn-Phong - používa halfway vector)
        vec3 viewDir = normalize(viewPosition - worldPosition.xyz);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 48.0);
        vec3 specular = spec * light.specular * light.intensity;

        // ATTENUATION
        float distance = length(light.position.xyz - worldPosition.xyz);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

        totalDiffuse += attenuation * diffuse;
        totalSpecular += attenuation * specular;
    }

    // FINAL COLOR
    vec3 result = globalAmbient + ambient + totalDiffuse + totalSpecular;
    out_Color = vec4(result, 1.0);
}