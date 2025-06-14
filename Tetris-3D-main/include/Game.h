#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "Tetromino.h"
#include "Board.h"
#include <map>
#include "shaderprogram.h"
#include "Score.h"

class Game {
public:
	Game(GLFWwindow* window, ShaderProgram* sp, glm::mat4 M, glm::mat4* V, glm::mat4* P, std::map<TetrominoType, GLuint>* textures);
	~Game();

	void render(glm::mat4 M, float angle_x, float nextTetrominoAngle);
	void update(bool timeReady, float* stepTime);

	void moveLeft(float angle_x);
	void moveRight(float angle_x);
	void moveForward(float angle_x);
	void moveBackward(float angle_x);

	TetrominoType getRandomTetrominoType();

	void tryRotateX();
	void tryRotateY();
private:
	GLFWwindow* window;
	ShaderProgram* sp;
	std::map<TetrominoType, GLuint>* textures;
	
	glm::mat4 V;
	glm::mat4 P;
	glm::mat4 M;

	std::vector<TetrominoType> bag;
	int currentIndex;
	void refillBag();

	Board* board;
	Tetromino* currentPiece;

	TetrominoType currentType;
	TetrominoType nextType;

	std::vector<std::vector<std::vector<bool>>> grid;
	std::vector<std::vector<std::vector<TetrominoType>>> texGrid;
	std::vector<glm::vec3> currentBlockCoords;

	glm::vec3 currentPosition;

	bool moving;
	bool justSpawned;

	Score* score;

	float dropInterval;

	glm::vec3 getWorldCoords(glm::vec3 coords);
	void spawnCurrentTetromino(glm::mat4 M);
            

	void spawnBoard(glm::mat4 M, float angle_x);
	void spawnStaticBlocks(glm::mat4 M);


	bool canPlace();
	void setStartingCoords();
	bool canMove();
	void decrementCoordY();
	void addTetrominoToGrid();

	bool canMoveByOffset(int dx, int dz);
	void moveByOffset(int dx, int dz, float angle_x);

	void clearFullLayers(float* stepTime);
	void spawnNextTetromino(float nextTetrominoAngle);

	std::vector<glm::vec3> rotateBlockCoordsY(const std::vector<glm::vec3>& coords);
	std::vector<glm::vec3> rotateBlockCoordsX(const std::vector<glm::vec3>& coords);
	bool canRotate(std::vector<glm::vec3> rotatedCoords);
};