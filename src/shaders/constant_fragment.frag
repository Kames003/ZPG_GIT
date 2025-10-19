#version 330

out vec4 out_Color;

uniform vec3 objectColor;

void main(void)
{
    // Konštantná farba - žiadne osvetlenie
    out_Color = vec4(objectColor, 1.0);
}