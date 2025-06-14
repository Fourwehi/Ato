#version 430 core

// Per-vertex inputs
layout (location = 0) in vec4 vertex;
layout (location = 1) in vec4 color;  // This will be set per-object via glVertexAttrib4fv
layout (location = 2) in vec4 normal;

// Uniforms
uniform mat4 P; // Projection
uniform mat4 V; // View
uniform mat4 M; // Model

// Outputs to Fragment Shader
out vec4 vColor;
out vec4 vNormal_vs; // Normal in view space
out vec4 vLightDir_vs; // Light direction in view space
out vec4 vViewDir_vs;  // View direction in view space

// Second light source outputs
out vec4 vNormal2_vs;
out vec4 vLightDir2_vs;
out vec4 vViewDir2_vs;

void main(void) {
    // Hard-coded light positions in world space
    vec4 lightPos1_ws = vec4(10.0, 20.0, -20.0, 1.0);
    vec4 lightPos2_ws = vec4(-20.0, 20.0, -20.0, 1.0);

    // Vertex position in view space
    vec4 vertex_vs = V * M * vertex;

    // --- Light 1 Calculations (in View Space) ---
    vec4 lightPos1_vs = V * lightPos1_ws;
    vLightDir_vs = normalize(lightPos1_vs - vertex_vs);
    vViewDir_vs = normalize(-vertex_vs); // The camera is at (0,0,0) in view space
    vNormal_vs = normalize(V * M * normal);

    // --- Light 2 Calculations (in View Space) ---
    vec4 lightPos2_vs = V * lightPos2_ws;
    vLightDir2_vs = normalize(lightPos2_vs - vertex_vs);
    vViewDir2_vs = normalize(-vertex_vs);
    vNormal2_vs = normalize(V * M * normal);
    
    // Pass the color attribute directly to the fragment shader
    vColor = color;

    // Final vertex position in clip space
    gl_Position = P * V * M * vertex;
}
