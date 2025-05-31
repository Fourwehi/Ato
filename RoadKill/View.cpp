//
// BetterCarryingEachOther 2016.03.17
// viewing area management
//

#include <GL/gl3w.h> // Changed from glew.h
// #include <GL/freeglut.h> // Removed
#include <iostream>
#include <cmath>

// #include "game.h" // Removed
#include "view.h" // Must be "View.h" to match filename
#include "utility.h"
// #include "mat.h" // Removed
#include "shader.h"
// #include "key.h" // Removed
#include <vmath.h>
#include "RoadKillApp.h" // For APP_GRID_SIZE and input methods
#include <GLFW/glfw3.h> // For GLFW_KEY_Q

#include "debug.h"

float View::x = 0.0f; // Added f
float View::y = 0.0f; // Added f
float View::width = 768.0f; // Added f
float View::height = 768.0f; // Added f
float View::tx = 0.0f; // Added f
float View::ty = 0.0f; // Added f

float View::offsetTop = View::height * 0.55f;
float View::offsetLeft = View::width * 0.5f;

float View::zfar = 1000.0f;
float View::znear = 10.0f;
float View::aspect = View::width / View::height; // Should be fine as width/height are float
float View::dist = 400.0f;

float View::viewDir = 0.0f;
View::mode View::viewMode = View::VIEW_FRONT;

void View::init() {
	x = 0.0f;
	y = -RoadKillApp::APP_GRID_SIZE * 5.0f; // Used RoadKillApp constant
	tx = View::width * 0.5f;
	ty = 0.0f;
}

void View::setViewAt(float _x, float _y) {
	tx = _x;
	ty = _y;
}

void View::setAspect(float asp) {
	aspect = asp;
}

void View::update(RoadKillApp* app) { // Signature changed
	if (app->is_key_first_pressed(GLFW_KEY_Q)) { // Changed to app input
		switch (viewMode) {
		case VIEW_FRONT: viewMode = VIEW_BACK; break;
		case VIEW_BACK: viewMode = VIEW_BIRD; break;
		case VIEW_BIRD: viewMode = VIEW_FRONT; break;
		}
	}

	x = flerp(tx, x, 0.9f);
	y = flerp(ty, y, 0.9f);
}

void View::draw() {
	vmath::mat4 projectionMatrix, viewMatrix; // Changed types
	vmath::vec3 eye_v3, at_v3, up_v3;    // Changed types

	switch (viewMode) {
	case VIEW_FRONT:
		eye_v3 = vmath::vec3(x, y, RoadKillApp::APP_GRID_SIZE / 2.0f);
		at_v3 = eye_v3 + vmath::vec3(cosf(vmath::radians(viewDir)), sinf(vmath::radians(viewDir)), 0.0f);
		up_v3 = vmath::vec3(0.0f, 0.0f, 1.0f);
		projectionMatrix = vmath::perspective(45.0f, aspect, znear, zfar);
		viewMatrix = vmath::lookat(eye_v3, at_v3, up_v3);
		break;
	case VIEW_BACK:
		at_v3 = vmath::vec3(x, y, RoadKillApp::APP_GRID_SIZE / 2.0f);
		eye_v3 = at_v3 - vmath::vec3(cosf(vmath::radians(viewDir)), sinf(vmath::radians(viewDir)), 0.0f) * dist;
		eye_v3 += vmath::vec3(0.0f, 0.0f, dist * 0.5f);
		up_v3 = vmath::vec3(0.0f, 0.0f, 1.0f);
		projectionMatrix = vmath::perspective(45.0f, aspect, znear, zfar);
		viewMatrix = vmath::lookat(eye_v3, at_v3, up_v3);
		break;
	case VIEW_BIRD:
		eye_v3 = vmath::vec3(0.0f, y, zfar * 0.2f); // Made 0.0f, 0.2f
		at_v3 = vmath::vec3(0.0f, y, -1.0f);    // Made 0.0f, -1.0f
		up_v3 = vmath::vec3(0.0f, 1.0f, 0.0f);     // Made 0.0f, 1.0f, 0.0f
		// Ortho parameters might need adjustment based on desired view volume
		projectionMatrix = vmath::ortho(
            -height / 3.0f * aspect, height / 3.0f * aspect,
            -height / 3.0f, height / 3.0f,
            -height, height); // Removed Game::getGrid() as it's not relevant for bird's eye Z
		viewMatrix = vmath::lookat(eye_v3, at_v3, up_v3);
		break;
	}
	Shader::getPhysicalShader().setProjection(projectionMatrix);
	Shader::getPhysicalShader().setEyeView(viewMatrix);
	Shader::getPhysicalShader().setEye(vmath::vec4(eye_v3, 1.0f)); // Pass vmath::vec4
}

void View::setViewDir(float dir) {
	viewDir = dir;
}
// ... (getWidth, getHeight, etc. methods remain the same) ...
float View::getWidth() { return width; }
float View::getHeight() { return height; }
float View::getX() { return x; }
float View::getY() { return y; }
float View::getZfar() { return zfar; }
float View::getZnear() { return znear; }
float View::getViewDir() { return viewDir; }
View::mode View::getViewMode() { return viewMode; }
