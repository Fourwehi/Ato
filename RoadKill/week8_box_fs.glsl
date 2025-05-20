//box_fs.glsl  
#version 430 core  

in vec3 vsColor;  
in vec2 vsTexCoord;  

uniform sampler2D texture1; // container  
uniform sampler2D texture2; // side  

out vec4 color;  

void main(void) {
    vec4 base = texture(texture1, vsTexCoord);
    color = base;

    /* vec4 overlay = texture(texture2, vsTexCoord);
    
    if(1.0 - overlay.r < 0.05 && 1.0 - overlay.g < 0.05 && 1.0 - overlay.b < 0.05) {
        color = base;
    } else {
        color = overlay;
    }
    */
    
    }