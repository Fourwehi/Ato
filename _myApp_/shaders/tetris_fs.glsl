#version 430 core

// Uniforms
uniform int mode; // 0 for simple color, 1 for lighting

// Inputs from Vertex Shader
in vec4 vColor;
in vec4 vNormal_vs;
in vec4 vLightDir_vs;
in vec4 vViewDir_vs;

in vec4 vNormal2_vs;
in vec4 vLightDir2_vs;
in vec4 vViewDir2_vs;

// Output color
out vec4 pixelColor;

void main(void) {
    if (mode == 0) {
        // Mode 0: Just use the passed-in color directly.
        // This is the primary mode for our Tetris game.
        pixelColor = vColor;
    } else if (mode == 1) {
        // Mode 1: Full lighting calculation (as in the original f_simplest.txt)
        // This mode can be used for more advanced visuals if desired.
        
        vec4 kd = vColor; // Diffuse color from vertex color
        vec4 ks = vec4(1.0); // Specular color is white
        float shininess = 50.0;

        // --- Light 1 ---
        vec4 N = normalize(vNormal_vs);
        vec4 L = normalize(vLightDir_vs);
        vec4 V = normalize(vViewDir_vs);
        vec4 R = reflect(-L, N);

        float diffuse1 = max(dot(N, L), 0.0);
        float specular1 = pow(max(dot(R, V), 0.0), shininess);

        // --- Light 2 ---
        vec4 N2 = normalize(vNormal2_vs);
        vec4 L2 = normalize(vLightDir2_vs);
        vec4 V2 = normalize(vViewDir2_vs);
        vec4 R2 = reflect(-L2, N2);

        float diffuse2 = max(dot(N2, L2), 0.0);
        float specular2 = pow(max(dot(R2, V2), 0.0), shininess);
        
        // Combine lighting results
        vec3 ambient = vec3(0.1) * kd.rgb;
        vec3 diffuse = kd.rgb * (diffuse1 + diffuse2);
        vec3 specular = ks.rgb * (specular1 + specular2);

        pixelColor = vec4(ambient + diffuse + specular, 1.0);
    }
}
