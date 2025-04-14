//box_vs.glsl
#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

uniform mat4 modelMatrix;
uniform mat4 LookAtMatrix;
uniform mat4 projectionMatrix;

out vec3 vsColor;

void main(void) {
    gl_Position =  projectionMatrix * LookAtMatrix * modelMatrix* vec4(pos, 1.0);
    vsColor = color;


}
