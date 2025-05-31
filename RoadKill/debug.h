#pragma once

#include <iostream>
#include <GL/gl3w.h> // Changed from glew.h
// #include <GL/freeglut.h> // Removed

inline void errorecho(const char* append) {
	std::cout << append << ": ";
	switch (glGetError()) {
	case GL_NO_ERROR:
		std::cout << "No error" << std::endl;
		break;
	case GL_INVALID_ENUM:
		std::cout << "Invalid enumerator" << std::endl;
		break;
	case GL_INVALID_VALUE:
		std::cout << "Invalid value" << std::endl;
		break;
	case GL_INVALID_OPERATION:
		std::cout << "Invalid operation" << std::endl;
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		std::cout << "Invalid framebuffer operation" << std::endl;
		break;
	case GL_OUT_OF_MEMORY:
		std::cout << "Out of memory" << std::endl;
		break;
	}
}

inline void errorecho() {
	errorecho("Error");
}
