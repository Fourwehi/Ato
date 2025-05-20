#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;     // 기존 color → normal
layout(location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vsPos;        // world-space position
out vec3 vsNormal;     // transformed normal
out vec2 vsTexCoord;   // 텍스쳐 좌표

void main(void) {
    // world-space 위치와 노멀 계산
    vsPos    = vec3(model * vec4(pos, 1.0));
    vsNormal = mat3(transpose(inverse(model))) * normal;
    vsTexCoord = texCoord;

    // 화면 변환
    gl_Position = projection * view * vec4(vsPos, 1.0);
}
