//fs.glsl (fragment shader)
#version 430 core

in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D tentFrontTexture;
uniform sampler2D tentSideTexture;
uniform sampler2D tentBackTexture;
uniform sampler2D groundTexture;
uniform sampler2D waterfallTexture;
uniform sampler2D wallTexture;
uniform sampler2D UTFrontTexture;
uniform sampler2D UTSideTexture;

uniform float textureOffset;


uniform int objectID;  // 0: tent, 1: wall, 2: ground 3: waterfall
uniform int faceID;



void main() {
   if (objectID == 0) {
        if (faceID == 0) {
            FragColor = texture(tentFrontTexture, vTexCoord);
        } else if (faceID == 1) {
            FragColor = texture(tentBackTexture, vTexCoord);
        } else {
            FragColor = texture(tentSideTexture, vTexCoord);
        }
        }
   else if (objectID == 1) {
        FragColor = texture(wallTexture, vTexCoord);
    }
  else if (objectID == 2) {
        FragColor = texture(groundTexture, vTexCoord);
    } else if (objectID == 3) {
        vec2 animatedCoord = vec2( fract(vTexCoord.x + textureOffset), vTexCoord.y);
        FragColor = texture(waterfallTexture, animatedCoord);
    } else  if (objectID == 4) {
        if (faceID == 0) {
            FragColor = texture(tentFrontTexture, vTexCoord);
        }  else {
            FragColor = texture(tentSideTexture, vTexCoord);
        }
      }
}