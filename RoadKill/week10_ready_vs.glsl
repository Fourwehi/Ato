#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;     // ���� color �� normal
layout(location = 2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vsPos;        // world-space position
out vec3 vsNormal;     // transformed normal
out vec2 vsTexCoord;   // �ؽ��� ��ǥ

void main(void) {
    // world-space ��ġ�� ��� ���
    vsPos    = vec3(model * vec4(pos, 1.0));
    vsNormal = mat3(transpose(inverse(model))) * normal;
    vsTexCoord = texCoord;

    // ȭ�� ��ȯ
    gl_Position = projection * view * vec4(vsPos, 1.0);
}
