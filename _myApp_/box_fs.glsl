//box_fs.glsl
#version 430 core

in vec3 vsColor;
in vec2 vsTexCoord;

uniform sampler2D texture1; // container
uniform sampler2D texture2; //side

out vec4 fragColor;

void main(void) {
   vec4 base = texture(texture1, vsTexCoord);
   vec4 overlay = texture(texture2, vsTexCoord);
if (overlay.r == 1.0 && overlay.g == 1.0 && overlay.b == 1.0) {
    fragColor = base;
} else {
    fragColor = overlay;
}
}