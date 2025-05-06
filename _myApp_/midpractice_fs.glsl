#version 430 core

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D tentFrontTexture;
uniform sampler2D tentSideTexture;
uniform sampler2D tentBackTexture;
uniform sampler2D groundTexture;
uniform sampler2D waterfallTexture;

uniform float textureOffset;

uniform int objectID; // 0: tent, 1: ground, 2: waterfall
uniform int faceID;

void main() {
    if (objectID == 0) {
        if (faceID == 0)
            FragColor = texture(tentFrontTexture, vTexCoord);
        else if (faceID == 1)
            FragColor = texture(tentBackTexture, vTexCoord);
        else
            FragColor = texture(tentSideTexture, vTexCoord);
    }
    else if (objectID == 1) {
        vec4 base = texture(groundTexture, vTexCoord);
        float xTex = vTexCoord.x;
        float waterfallX = 3.0;
        float dist = abs(xTex - waterfallX);

        float gradient = smoothstep(1.0, 0.0, dist); // 1.0거리보다 가까우면 블렌딩 증가
        vec4 blueTint = vec4(0.0, 0.0, 1.0, 1.0);

        FragColor = mix(base, blueTint, gradient);
    }
    else if (objectID == 2) {
        vec2 animatedCoord = vec2(vTexCoord.x, fract(vTexCoord.y + textureOffset));
        FragColor = texture(waterfallTexture, animatedCoord);
    }
}
