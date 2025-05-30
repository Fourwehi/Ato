//
// BetterCarryingEachOther 2016.04.01
// shader management
//

#pragma once

#include <GL/glew.h>
#include <stack>
#include <vector>

#include <vmath.h>

#define PSHADER_NUM 4

using namespace std;

class PhysicalShader {
private:
	GLuint program;

	GLuint projection;
	GLuint eyeview;
	GLuint modelview;

	GLuint lightAmbient;
	GLuint lightCount;
	GLuint lightColor;
	GLuint lightPosition;
	GLuint lightEye;
	GLuint specularPower;
	GLuint specularLightness;

	GLuint uvOffset;

public:
	// void init(const char* fv, const char* fg, const char* ff); // Old init
	void init(GLuint program_id); // New init
	void use() { glUseProgram(program); }
	GLuint getProjection() { return projection; }
	GLuint getModelView() { return modelview; }
	void setLights(int count, GLfloat* positions, GLfloat* colors);
	void setEye(vmath::vec4& eye);
	void setAmbient(vmath::vec4& color);
	void setModelView(vmath::mat4& mat);
	void setEyeView(vmath::mat4& mat);
	void setProjection(vmath::mat4& mat);
	void setSpecular(float s);
	void setUVOffset(vmath::vec2& uv);

	GLuint getLightAmbient() { return lightAmbient; }
};

class Shader {
private:
	// These are now loaded by RoadKillApp and passed to Shader class
	// static GLuint shdFramePass;
	// static GLuint shdFog;
	// static GLuint shdSSAO;
	// static GLuint shdBlur;

	// Static members to store program IDs
    static GLuint s_program_gouraud;
    static GLuint s_program_flat;
    static GLuint s_program_hide;
    static GLuint s_program_framepass;
    static GLuint s_program_fog;
    static GLuint s_program_blur;
    static GLuint s_program_ssao;

	static PhysicalShader pshader[PSHADER_NUM];
	static PhysicalShader hide;
	static int pshaderCurrent;
	static vmath::mat4 modelViewCurrent; // Needs initialization, e.g. = vmath::mat4::identity(); in .cpp or a static init block
	static stack<vmath::mat4> modelViewStack;
	static vector<vmath::vec4> lightPositions;
	static vector<vmath::vec4> lightColors;
	static int lightCount;
	static GLuint fogColor;
	static bool hidePass;

public:
	static void initialize_shader_programs(GLuint pg, GLuint pf, GLuint ph, GLuint pfp, GLuint pfog, GLuint pblur, GLuint pssao);
	static void init();

	static void usePhysicalShader() { pshader[pshaderCurrent].use(); hidePass = false; }
	static void switchPhysicalShader() { pshaderCurrent = (pshaderCurrent + 1) % PSHADER_NUM; }
	static void useFramePass() { glUseProgram(s_program_framepass); }
	static void useFog() { glUseProgram(s_program_fog); }
	static void useHide() { hide.use(); hidePass = true; } // hide is a PhysicalShader, uses its own program ID
	static void useSSAO() { if(s_program_ssao != 0) glUseProgram(s_program_ssao); } // Check if SSAO loaded
	static void useBlur() { glUseProgram(s_program_blur); }
	static bool isWire() { return (pshaderCurrent == 3); }

	static void translate(vmath::vec3 pos) { modelViewCurrent = modelViewCurrent * vmath::translate(pos); }
	static void rotateX(float a) { modelViewCurrent = modelViewCurrent * vmath::rotate(vmath::radians(a), vmath::vec3(1.0f, 0.0f, 0.0f)); }
	static void rotateY(float a) { modelViewCurrent = modelViewCurrent * vmath::rotate(vmath::radians(a), vmath::vec3(0.0f, 1.0f, 0.0f)); }
	static void rotateZ(float a) { modelViewCurrent = modelViewCurrent * vmath::rotate(vmath::radians(a), vmath::vec3(0.0f, 0.0f, 1.0f)); }
	static void scale(vmath::vec3 sv) { modelViewCurrent = modelViewCurrent * vmath::scale(sv); }

	static void pop();
	static void push();
	static void apply() { getPhysicalShader().setModelView(modelViewCurrent); }

	static void shift(vmath::vec2 uv) { getPhysicalShader().setUVOffset(uv); }

	static void lightPush(vmath::vec4& position, vmath::vec4& color);
	static void lightApply();
	static void lightClear();

	static void fogSetColor(vmath::vec4& color);

	static PhysicalShader& getPhysicalShader();
};