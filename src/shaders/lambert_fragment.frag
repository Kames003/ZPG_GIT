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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;  // TODO nutne ju tu nepotrebuješ
    float intensity;
};

// ========================================
// UNIFORMS
// ========================================
uniform vec3 objectColor;
uniform Light lights[MAX_LIGHTS];
uniform int numberOfLights;

// ========================================
// MAIN - LAMBERT SHADING
// ========================================
void main(void)
{
    //  GLOBÁLNE AMBIENT - iba raz, mimo cyklu
    vec3 globalAmbient = objectColor * 0.15;

    //  AMBIENT z prvého svetla - LEN RAZ!
    vec3 ambient = vec3(0.0);
    if (numberOfLights > 0) {
        ambient = lights[0].ambient * objectColor * 0.1;
    }

    // Akumulácia diffuse
    vec3 totalDiffuse = vec3(0.0);

    //  Cyklus cez všetky svetlá - LEN diffuse!
    for (int i = 0; i < numberOfLights; i++) {
        Light light = lights[i];

        // DIFFUSE (Lambertian reflection)
        vec3 norm = normalize(worldNormal);
        vec3 lightDir = normalize(light.position.xyz - worldPosition.xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * objectColor * light.diffuse * light.intensity;

        // ATTENUATION
        float distance = length(light.position.xyz - worldPosition.xyz);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

        totalDiffuse += attenuation * diffuse;
    }

    //  FINAL COLOR
    vec3 result = globalAmbient + ambient + totalDiffuse;
    out_Color = vec4(result, 1.0);
}