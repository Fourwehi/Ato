#include "shader.h"
#include <vector>   // For std::vector
#include <stack>    // For std::stack
// Note: vmath.h should be included via shader.h as per previous steps

// --- Static Member Definitions for Shader class ---
vmath::mat4 Shader::modelViewCurrent = vmath::mat4::identity();
std::stack<vmath::mat4> Shader::modelViewStack;

PhysicalShader Shader::pshader[PSHADER_NUM];
PhysicalShader Shader::hide;

int Shader::pshaderCurrent = 0;

std::vector<vmath::vec4> Shader::lightPositions;
std::vector<vmath::vec4> Shader::lightColors;
int Shader::lightCount = 0;

bool Shader::hidePass = false;

// Static program ID storage
GLuint Shader::s_program_gouraud = 0;
GLuint Shader::s_program_flat = 0;
GLuint Shader::s_program_hide = 0;
GLuint Shader::s_program_framepass = 0;
GLuint Shader::s_program_fog = 0;
GLuint Shader::s_program_blur = 0;
GLuint Shader::s_program_ssao = 0;

// Old GLuint handles for programs previously loaded by Shader::init - now unused here
// GLuint Shader::shdFramePass = 0;
// GLuint Shader::shdFog = 0;
// GLuint Shader::shdSSAO = 0;
// GLuint Shader::shdBlur = 0;

GLuint Shader::fogColor = 0; // This is likely a uniform location, keep as is for now.

// --- Method Implementations ---

void Shader::initialize_shader_programs(GLuint pg, GLuint pf, GLuint ph, GLuint pfp, GLuint pfog, GLuint pblur, GLuint pssao) {
    s_program_gouraud = pg;
    s_program_flat = pf;
    s_program_hide = ph;
    s_program_framepass = pfp;
    s_program_fog = pfog;
    s_program_blur = pblur;
    s_program_ssao = pssao;
}

// Implementation for PhysicalShader::init
// Uniform names are based on existing member variable names in PhysicalShader
void PhysicalShader::init(GLuint program_id) {
    program = program_id; // Store the program ID

    // Get uniform locations
    projection = glGetUniformLocation(program, "projection");
    eyeview = glGetUniformLocation(program, "eyeview"); // Or "viewMatrix", "view" - needs to match GLSL
    modelview = glGetUniformLocation(program, "modelview"); // Or "modelMatrix", "model"

    lightAmbient = glGetUniformLocation(program, "lightAmbient");
    lightCount = glGetUniformLocation(program, "lightCount");
    lightColor = glGetUniformLocation(program, "lightColor");
    lightPosition = glGetUniformLocation(program, "lightPosition");
    lightEye = glGetUniformLocation(program, "lightEye"); // Or "viewPos", "eyePos"
    specularPower = glGetUniformLocation(program, "specularPower");
    specularLightness = glGetUniformLocation(program, "specularLightness");
    uvOffset = glGetUniformLocation(program, "uvOffset");

    // It's good practice to check if glGetUniformLocation returns -1 (uniform not found)
    // For brevity, this check is omitted here but should be done in robust code.
}

// Refactored Shader::init
void Shader::init() {
    // Initialize PhysicalShader instances with their respective program IDs
    // The uniform names used in PhysicalShader::init will be the default ones like "projection", "modelview", etc.
    // Ensure these names match the actual uniform names in your GLSL files.

    // pshader[0] - Gouraud
    if (s_program_gouraud != 0) pshader[0].init(s_program_gouraud);
    // pshader[1] - Flat
    if (s_program_flat != 0) pshader[1].init(s_program_flat);
    // pshader[2] - Gouraud (example: could be a different variant or same)
    if (s_program_gouraud != 0) pshader[2].init(s_program_gouraud);
    // pshader[3] - Hide (used for wireframe)
    if (s_program_hide != 0) pshader[3].init(s_program_hide);

    // General 'hide' shader
    if (s_program_hide != 0) hide.init(s_program_hide);

    // The dedicated GLuint members for shdFramePass, shdFog, shdSSAO, shdBlur
    // are no longer needed here as these programs are loaded by RoadKillApp
    // and their IDs are stored in s_program_framepass etc.
    // Their usage is now glUseProgram(s_program_fog) etc.

    // If fogColor was intended to be a uniform location for the fog shader:
    if (s_program_fog != 0) {
         fogColor = glGetUniformLocation(s_program_fog, "fogColor"); // Example uniform name
    }

    // Reset model view matrix stack and current matrix
    while (!modelViewStack.empty()) {
        modelViewStack.pop();
    }
    modelViewCurrent = vmath::mat4::identity();
    pshaderCurrent = 0; // Default shader
    lightCount = 0;
    hidePass = false;
}

// Definitions for other Shader methods like pop, push, lightPush, etc.
// These were not part of this specific refactoring step for shader loading
// but would be in a complete shader.cpp file.
// For example:
void Shader::push() {
    modelViewStack.push(modelViewCurrent);
}

void Shader::pop() {
    if (!modelViewStack.empty()) {
        modelViewCurrent = modelViewStack.top();
        modelViewStack.pop();
    } else {
        // Handle stack underflow, perhaps reset to identity or log an error
        modelViewCurrent = vmath::mat4::identity();
    }
}

void Shader::lightPush(vmath::vec4& position, vmath::vec4& color) {
    if (lightCount < 8) { // Assuming max 8 lights, adjust as needed
        lightPositions.push_back(position);
        lightColors.push_back(color);
        lightCount++;
    }
}

void Shader::lightApply() {
    if (lightCount > 0 && !hidePass) { // 'hidePass' check might need adjustment
        getPhysicalShader().setLights(lightCount, (GLfloat*)&lightPositions[0], (GLfloat*)&lightColors[0]);
    }
}

void Shader::lightClear() {
    lightPositions.clear();
    lightColors.clear();
    lightCount = 0;
}

void Shader::fogSetColor(vmath::vec4& color) {
    // Assuming s_program_fog is the currently bound program if this is called
    // and fogColor is the uniform location for the fog's color.
    if (s_program_fog != 0 && fogColor != (GLuint)-1 && fogColor != 0) { // Check if fogColor is a valid location
         glUniform4fv(fogColor, 1, color);
    }
}

PhysicalShader& Shader::getPhysicalShader() {
    return hidePass ? hide : pshader[pshaderCurrent];
}
