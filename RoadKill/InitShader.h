//
// BetterCarryingEachOther 2016.05.09
// refactored initshader module (header-only with inline)
//

#pragma once

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "debug.h"

inline void loadShader(GLuint program, GLenum type, const char* fn) {
	std::string buf;
	std::string source;
	std::ifstream f(fn);

	if (!f.is_open()) {
		std::cerr << "Failed to read " << fn << std::endl;
		exit(EXIT_FAILURE);
	}

	while (!f.eof()) {
		std::getline(f, buf);
		source += buf + "\n";
	}

	const char* csource = source.c_str();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&csource, NULL);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		std::cerr << fn << " failed to compile:" << std::endl;
		GLint logSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetShaderInfoLog(shader, logSize, NULL, logMsg);
		std::cerr << logMsg << std::endl;
		delete[] logMsg;
		exit(EXIT_FAILURE);
	}

	glAttachShader(program, shader);
}

inline void linkProgram(GLuint program) {
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		std::cerr << "Shader program failed to link" << std::endl;
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		std::cerr << logMsg << std::endl;
		delete[] logMsg;
		exit(EXIT_FAILURE);
	}

	glUseProgram(program);
}
