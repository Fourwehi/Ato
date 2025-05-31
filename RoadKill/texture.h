//
// BetterCarryingEachOther 2016.04.07
// texture management unit
//

#pragma once

#include <GL/gl3w.h> // Changed from glew.h
// #include <GL/freeglut.h> // Removed

class Texture {
private:
	unsigned w, h;
	GLuint buf;
	GLenum type;

public:
	Texture() : w(0), h(0), buf(0), type(0) {} // Initialize members
	void load(const char* fn, GLenum type);
	void bind();
	void nbind();
	void cleanup();
};