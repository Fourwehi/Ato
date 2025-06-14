#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec4 vertex;
in vec4 color;
in vec4 normal;
in vec2 texCoord0;

out vec4 vColor;
out vec2 iTexCoord0;

out vec4 vNormal; // normal in world coordinates
out vec4 vLightDir; // light direction in world coordinates
out vec4 vViewDir; // view direction in world coordinates

out vec4 vNormal2;
out vec4 vLightDir2;
out vec4 vViewDir2;

void main(void) {
    vec4 lightPos = vec4(0, 30, -35, 1); // œwiat³o w przestrzeni œwiata
    vLightDir = normalize(V * lightPos - V * M * vertex); // jednostkowy wektor kierunku swiatla w przestrzeni kamery
    vViewDir = normalize(vec4(0, 0, 0, 1) - V * M * vertex); // jednostkowy wektor kierunku swiatla w przestrzeni kamery
    vNormal = normalize(V * M * normal); // normalna jednostkowa w przestrzeni kamery

    vec4 lightPos2 = vec4(0, 30, 0, 1); // œwiat³o w przestrzeni œwiata
    vLightDir2 = normalize(V * lightPos2 - V * M * vertex);
    vViewDir2 = normalize(vec4(0, 0, 0, 1) - V * M * vertex);
    vNormal2 = normalize(V * M * normal);

    vColor = color;
    iTexCoord0 = texCoord0;
    gl_Position = P * V * M * vertex;
}