//box_fs.glsl
#version 430 core

in vec3 vsColor;
in vec2 vsTexCoord;

uniform sampler2D textures[6];

out vec4 fragColor;

void main(void) {
    fragColor = texture(textures[0], vsTexCoord);
}
