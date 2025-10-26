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
    // ✅ VEĽMI NÍZKY AMBIENT - pre sivý, tmavý vzhľad ako na referenčnom obrázku
    vec3 globalAmbient = objectColor * 0.08;  // Minimálny ambient

    vec3 ambient = vec3(0.0);
    if (numberOfLights > 0) {
        ambient = lights[0].ambient * objectColor * 0.05;
    }

    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    for (int i = 0; i < numberOfLights; i++) {
        Light light = lights[i];

        // DIFFUSE
        vec3 norm = normalize(worldNormal);
        vec3 lightDir = normalize(light.position.xyz - worldPosition.xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * objectColor * light.diffuse * light.intensity;

        // SPECULAR - stredná ostrosť odleskov
        vec3 viewDir = normalize(viewPosition - worldPosition.xyz);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 40.0);  // Striedna ostrosť
        vec3 specular = spec * light.specular * light.intensity;

        // ATTENUATION - kritické pre správny útlum!
        float distance = length(light.position.xyz - worldPosition.xyz);
        float attenuation = 1.0 / (1.0 + 0.14 * distance + 0.07 * distance * distance);

        totalDiffuse += attenuation * diffuse;
        totalSpecular += attenuation * specular;
    }

    // FINAL COLOR
    vec3 result = globalAmbient + ambient + totalDiffuse + totalSpecular;
    out_Color = vec4(result, 1.0);
}