#version 330

in vec4 worldPosition; // Pozícia bodu na guľi (v world space)
in vec3 worldNormal; // normála v danom bode 

out vec4 out_Color;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 objectColor;

void main(void)
{
    // AMBIENT - nie je ovplyvnený útlmom
    vec3 ambient = 0.25 * objectColor;

    // difúzna zložka - závisí od uhla medzi normálou a svetlom
    vec3 norm = normalize(worldNormal);
    vec3 lightDir = normalize(lightPosition - worldPosition.xyz);
    //              ↑
    //        Smer OD bodu na guli K svetlu
    float diff = max(dot(norm, lightDir), 0.0);
    //           ↑
    //     Ak je normála a svetlo v rovnakom smere → diff = 1.0
    //     Ak sú kolmé → diff = 0.0
    //     Ak sú opačne → diff = 0.0 (max zabezpečí, že nebude záporné)
    vec3 diffuse = diff * objectColor;

    // SPECULAR (Phong)
    vec3 viewDir = normalize(viewPosition - worldPosition.xyz);
    vec3 reflectDir = reflect(-lightDir, norm);
    //                ↑
    //          Smer odrazeného svetla
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 24.0);
    //     Ak sa dívaš na odraz → spec = 1.0           Shininess (ostrosť)

    vec3 specular = 1.2 * spec * vec3(1.0);

    // ← ATTENUATION (útlm svetla so vzdialenosťou)
    float distance = length(lightPosition - worldPosition.xyz);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    // FINAL - ambient NIE JE útlmený, diffuse a specular ÁNO
    vec3 result = ambient + attenuation * (diffuse + specular);
    out_Color = vec4(result, 1.0);
}