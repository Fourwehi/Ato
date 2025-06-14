#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shaderprogram.h"
#include <iostream>
#include <map>
#include <vector>

enum class TetrominoType {
    HERO, L1, L2, Z1, Z2, TEEWEE, SMASHBOY, None
};

extern std::map<TetrominoType, std::vector<glm::vec3>> tetrominoShapes;

class Tetromino {
public:
    Tetromino(GLFWwindow* window, ShaderProgram* sp, glm::mat4* V, glm::mat4* P);
	~Tetromino();

    void drawCube(glm::mat4 blockMatrix, GLuint tex, glm::vec3 blockPosition);
    static std::string tetrominoTypeToString(TetrominoType type);
private:
    GLFWwindow* window;
    ShaderProgram* sp;
    glm::mat4* P;
    glm::mat4* V;

    void renderCube(glm::mat4 blockMatrix, GLuint tex);
};