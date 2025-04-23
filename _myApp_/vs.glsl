//vs.glsl (vertex shader)
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 mvpM;

out vec2 vTexCoord;

void main() {
    gl_Position = mvpM * vec4(position, 1.0);
    vTexCoord = texCoord;
}
