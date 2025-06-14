#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <vector>
#include "shaderprogram.h"
#include "Tetromino.h"
#include "cube.h"
#include "readTexture.h"
#include <iostream>

std::map<TetrominoType, std::vector<glm::vec3>> tetrominoShapes = {
    { TetrominoType::HERO, {{-4, 0, 0}, {-2, 0, 0}, {0, 0, 0}, {2, 0, 0}} },
    { TetrominoType::L1, {{-2, -2, 0}, {0, -2, 0}, {2, -2, 0}, {2, 0, 0}} },
    { TetrominoType::L2, {{-2, 0, 0}, {0, -2, 0}, {-2, -2, 0}, {2, -2, 0}} },
    { TetrominoType::Z1, {{-2, 0, 0}, {0, -2, 0} , {0, 0, 0}, {2, -2, 0}} },
    { TetrominoType::Z2, {{-2, -2, 0}, {0, -2, 0}, {0, 0, 0}, {2, 0, 0}} },
    { TetrominoType::TEEWEE, {{-2, -2, 0}, {0, -2, 0}, {0, 0, 0}, {2, -2, 0}} },
    { TetrominoType::SMASHBOY, {{-2, 0, 0}, {-2, -2, 0}, {0, 0, 0}, {0, -2, 0}} }
};

Tetromino::Tetromino(GLFWwindow* window, ShaderProgram* sp, glm::mat4* V, glm::mat4* P) {
    this->window = window;
    this->sp = sp;
    this->V = V;
    this->P = P;
}

Tetromino::~Tetromino() {}

void Tetromino::drawCube(glm::mat4 blockMatrix, GLuint tex, glm::vec3 blockPosition) {    
    blockMatrix = glm::translate(blockMatrix, blockPosition);
    renderCube(blockMatrix, tex);
}

void Tetromino::renderCube(glm::mat4 blockMatrix, GLuint tex) {
    sp->use();
    glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(*P));
    glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(*V));
    glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(blockMatrix));

    glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
    glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, cubeVertices); //Wska¿ tablicê z danymi dla atrybutu vertex

    glEnableVertexAttribArray(sp->a("normal"));  //W³¹cz przesy³anie danych do atrybutu normal
    glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, cubeNormals); //Wska¿ tablicê z danymi dla atrybutu normal

    glEnableVertexAttribArray(sp->a("texCoord0"));  //W³¹cz przesy³anie danych do atrybutu texCoord0
    glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, cubeTexCoords); //Wska¿ tablicê z danymi dla atrybutu texCoord0

    glUniform1i(sp->u("mode"), 1);
    glUniform1i(sp->u("textureMap0"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount); //Narysuj obiekt

    glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
    glDisableVertexAttribArray(sp->a("normal"));  //Wy³¹cz przesy³anie danych do atrybutu normal
    glDisableVertexAttribArray(sp->a("texCoord0"));  //Wy³¹cz przesy³anie danych do atrybutu texCoord0
}

std::string Tetromino::tetrominoTypeToString(TetrominoType type) {
    switch (type) {
    case TetrominoType::HERO: return "HERO";
    case TetrominoType::L1: return "L1";
    case TetrominoType::L2: return "L2";
    case TetrominoType::Z1: return "Z1";
    case TetrominoType::Z2: return "Z2";
    case TetrominoType::TEEWEE: return "TEEWEE";
    case TetrominoType::SMASHBOY: return "SMASHBOY";
	case TetrominoType::None: return "None";
    default: return "UNKNOWN";
    }
}