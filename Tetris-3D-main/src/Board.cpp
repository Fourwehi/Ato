#include "Board.h"
#include "square.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "constants.h"
#define SQUARE_SIZE 2

Board::Board(GLFWwindow* window, ShaderProgram* sp, glm::mat4 *game, glm::mat4* V, glm::mat4* P, int base, int height) {
    this->window = window;
    this->sp = sp;
    this->game = game;
    this->P = P;
    this->V = V;
    this->base = base;
    this->height = height;
}

Board::~Board() {}

void Board::drawBoard(glm::mat4 M, float angle_x) {
	glm::mat4 baseMatrix = glm::translate(M, glm::vec3(-base * SQUARE_SIZE / 2.0f + 1, 0.0f, -base * SQUARE_SIZE / 2.0f + 1));
	baseMatrix = glm::rotate(baseMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    drawBase(baseMatrix);
    glm::mat4 wallsMatrix = M;
    drawWalls(wallsMatrix, angle_x);
}

void Board::drawSquare(glm::mat4 matrix) {
    sp->use();

    glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(*P));
    glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(*V));
    glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(matrix));

    glUniform1i(sp->u("mode"), 0);

    glEnableVertexAttribArray(sp->a("vertex"));
    glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, squareVertices);

    glEnableVertexAttribArray(sp->a("color"));
    glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, squareColors);

    glDrawElements(GL_LINES, squareIndexCount, GL_UNSIGNED_INT, squareIndexes);

    glDisableVertexAttribArray(sp->a("vertex"));
    glDisableVertexAttribArray(sp->a("color"));
}

void Board::drawBase(glm::mat4 gameMatrix) {
    glm::mat4 baseMatrix = gameMatrix;
    for (int i = 0; i < base; i++) {
        for (int j = 0; j < base; j++) {
			glm::mat4 currentMatrix = glm::translate(baseMatrix, glm::vec3(SQUARE_SIZE * i, SQUARE_SIZE * j, 0.0f));
            drawSquare(currentMatrix);
        }
    }
}

void Board::drawWall(glm::mat4 wallTransform) {
    for (int j = 0; j < height; j++) {
        for (int k = 0; k < base; k++) {
            glm::mat4 squareMatrix = glm::translate(wallTransform, glm::vec3(k * SQUARE_SIZE, j * SQUARE_SIZE, 0.0f));
            drawSquare(squareMatrix);
        }
    }
}

void Board::drawWalls(glm::mat4 wallsMatrix, float angle_x) {
    float border = 0.17;
    glm::mat4 wallTransform = wallsMatrix;

    if (angle_x <= border || angle_x >= (PI - border)) {
        // lewa sciana
        wallTransform = glm::rotate(wallTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wallTransform = glm::translate(wallTransform, glm::vec3(-base * SQUARE_SIZE / 2.0f + 1, 1.0f, base * SQUARE_SIZE / 2.0f));
        drawWall(wallTransform);
    }
    wallTransform = wallsMatrix;
    if (angle_x <= (border + PI) || angle_x >= (PI - border + PI)) {
        // prawa sciana
        wallTransform = glm::rotate(wallTransform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wallTransform = glm::translate(wallTransform, glm::vec3(-base * SQUARE_SIZE / 2.0f + 1, 1.0f, -base * SQUARE_SIZE / 2.0f));
        drawWall(wallTransform);
    }
    wallTransform = wallsMatrix;
    if (angle_x <= (border + PI * 0.5) || angle_x >= (PI - border + PI * 0.5)) {
        // tylnia sciana
        wallTransform = glm::rotate(wallTransform, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wallTransform = glm::translate(wallTransform, glm::vec3(-base * SQUARE_SIZE / 2.0f + 1, 1.0f, base * SQUARE_SIZE / 2.0f));
        drawWall(wallTransform);
    }
    wallTransform = wallsMatrix;
    if (angle_x <= (border + PI * 1.5) && angle_x >= (PI - border - PI * 0.5)) {
        // przednia sciana
        wallTransform = glm::rotate(wallTransform, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wallTransform = glm::translate(wallTransform, glm::vec3(-base * SQUARE_SIZE / 2.0f + 1, 1.0f, -base * SQUARE_SIZE / 2.0f));
        drawWall(wallTransform);
    }
}
