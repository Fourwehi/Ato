//box_vs.glsl
#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout(location=2) in vec2 texCoord;

uniform mat4 mvpM;

out vec3 vsColor;
out vec2 vsTexCoord;

void main(void) {
    gl_Position =  mvpM* vec4(pos, 1.0);
    vsColor = color;
    vsTexCoord = texCoord;

}