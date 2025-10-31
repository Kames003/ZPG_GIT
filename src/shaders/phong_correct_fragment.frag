#version 330

in vec4 worldPosition;
in vec3 worldNormal;
out vec4 out_Color;

#define MAX_LIGHTS 8

struct Light {
    vec4 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};

uniform vec3 viewPosition;
uniform vec3 objectColor;
uniform Light lights[MAX_LIGHTS];
uniform int numberOfLights;

void main(void)
{
    // Vyšší ambient pre viditeľnosť
    vec3 globalAmbient = objectColor * 0.15;

    vec3 ambient = vec3(0.0);
    if (numberOfLights > 0) {
        ambient = lights[0].ambient * objectColor * 0.1;
    }

    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    for (int i = 0; i < numberOfLights; i++) {
        Light light = lights[i];

        vec3 norm = normalize(worldNormal);
        vec3 lightDir = normalize(light.position.xyz - worldPosition.xyz);


        float dotNL = dot(norm, lightDir);

        if (dotNL > 0.0) {
            // LEN ak je normála smerom k svetlu
            vec3 diffuse = dotNL * objectColor * light.diffuse * light.intensity;

            vec3 viewDir = normalize(viewPosition - worldPosition.xyz);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = spec * light.specular * light.intensity * 0.5;

            totalDiffuse += diffuse;
            totalSpecular += specular;
        }
        // Ak dotNL <= 0 → ŽIADNE osvetlenie
    }

    vec3 result = globalAmbient + ambient + totalDiffuse + totalSpecular;
    out_Color = vec4(result, 1.0);
}