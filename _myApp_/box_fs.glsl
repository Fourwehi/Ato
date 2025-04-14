//box_fs.glsl
#version 430 core

in vec3 vsColor;
out vec4 fragColor;

void main(void) {
    fragColor = vec4(vsColor, 1.0);
}
