#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shaderprogram.h"

class Board {
public:
    Board(GLFWwindow* window, ShaderProgram* sp, glm::mat4* game, glm::mat4* V, glm::mat4* P, int base, int height);
    ~Board();

    void drawBoard(glm::mat4 M, float angle_x);

private:
    GLFWwindow* window;
    ShaderProgram* sp;
    glm::mat4* P;
    glm::mat4* V;
    glm::mat4* game;
    int base;
    int height;

    void drawSquare(glm::mat4 baseMatrix);
    void drawBase(glm::mat4 gameMatrix);
    void drawWalls(glm::mat4 wallsMatrix, float angle_x);
    void drawWall(glm::mat4 wallTransform);
};
