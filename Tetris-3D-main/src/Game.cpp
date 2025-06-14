#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "Board.h"
#include "Tetromino.h"
#include "readTexture.h"
#include <map>
#include <iostream>
#include "Game.h"

#include <algorithm>
#include <vector>       
#include <random> 
#include "Score.h"


const int GRID_WIDTH = 5;
const int GRID_DEPTH = 5;
const int GRID_HEIGHT = 12;
const int BLOCK_SIZE = 2;

std::map<TetrominoType, int> tetrominoPoints = {
	{ TetrominoType::HERO, 5 },
	{ TetrominoType::L1, 3 },
	{ TetrominoType::L2, 3 },
	{ TetrominoType::Z1, 4 },
	{ TetrominoType::Z2, 4 },
	{ TetrominoType::TEEWEE, 6 },
	{ TetrominoType::SMASHBOY, 10 }
};

Game::Game(GLFWwindow* window, ShaderProgram* sp, glm::mat4 M, glm::mat4* V, glm::mat4* P, std::map<TetrominoType, GLuint>* textures) {
	this->window = window;
	this->sp = sp;
	this->M = M;
	this->V = *V;
	this->P = *P;
	this->textures = textures;

	this->grid.resize(GRID_HEIGHT, std::vector<std::vector<bool>>(GRID_WIDTH, std::vector<bool>(GRID_DEPTH, false))); // y x z
	this->texGrid.resize(GRID_HEIGHT, std::vector<std::vector<TetrominoType>>(GRID_WIDTH, std::vector<TetrominoType>(GRID_DEPTH, TetrominoType::None))); // y x z
	this->currentBlockCoords.resize(4, glm::vec3(0.0f)); // y x z

	this->nextType = getRandomTetrominoType();

	this->board = new Board(window, sp, &M, V, P, GRID_WIDTH, GRID_HEIGHT);
	this->currentPiece = new Tetromino(window, sp, V, P);

	this->moving = false;
	this->justSpawned = false;

	this->score = new Score();

	refillBag();
};

Game::~Game() {
	if (board) delete board;
	if (currentPiece) delete currentPiece;
	delete score;
};

glm::vec3 Game::getWorldCoords(glm::vec3 coords) {
	coords.x = (coords.x - GRID_WIDTH / 2) * BLOCK_SIZE;;
	coords.y = coords.y * BLOCK_SIZE + 1;;
	coords.z = (coords.z - GRID_DEPTH / 2) * BLOCK_SIZE;

	return coords;
};

void Game::spawnCurrentTetromino(glm::mat4 M) {
	if (!moving) return;
	for (int i = 0; i < 4; i++) {
		glm::vec3 blockPosition = getWorldCoords(currentBlockCoords[i]);
		this->currentPiece->drawCube(M, (*textures)[currentType], blockPosition);
	}
}

void Game::spawnBoard(glm::mat4 M, float angle_x) {
	board->drawBoard(M, angle_x);
}

void Game::spawnStaticBlocks(glm::mat4 M) {
	glm::mat4 startingPosition = glm::translate(M, glm::vec3(-GRID_WIDTH / 2, 1, -GRID_DEPTH / 2));
	glm::vec3 block = glm::vec3(0.0f, 0.0f, 0.0f);
	new Tetromino(window, sp, &V, &P);
	for (int i = 0; i < GRID_HEIGHT; i++) {
		for (int j = 0; j < GRID_WIDTH; j++) {
			for (int k = 0; k < GRID_DEPTH; k++) {
				if (this->grid[i][j][k]) { // y x z
					block = getWorldCoords(glm::vec3(j, i, k)); // x y z
					this->currentPiece->drawCube(M, (*textures)[texGrid[i][j][k]], block);
				}
			}
		}
	}
}

bool Game::canPlace() {
	for (int i = 0; i < 4; i++) { // y x z
		if (grid[currentBlockCoords[i].y][currentBlockCoords[i].x][currentBlockCoords[i].z]) {
			return false;
		}
	}
	return true;
}

void Game::setStartingCoords() {
	for (int i = 0; i < 4; i++) {
		this->currentBlockCoords[i].y = GRID_HEIGHT - 1 + (tetrominoShapes[this->currentType][i][1] / 2); // x y z
		this->currentBlockCoords[i].x = (GRID_WIDTH / 2) + (tetrominoShapes[this->currentType][i][0] / 2);
		this->currentBlockCoords[i].z = (GRID_DEPTH / 2) + (tetrominoShapes[this->currentType][i][2] / 2);
		//std::cout << "###coord###: x=" << currentBlockCoords[i].x << ", y=" << currentBlockCoords[i].y << ", z=" << currentBlockCoords[i].z << std::endl;
	}
}

bool Game::canMove() {
	for (int i = 0; i < 4; i++) {
		int x = currentBlockCoords[i].x;
		int y = currentBlockCoords[i].y;
		int z = currentBlockCoords[i].z;

		if (y <= 0) return false; // blok uderzy dno

		// sprawdŸ, czy wspó³rzêdne po przesuniêciu s¹ w zakresie
		if (x < 0 || x >= GRID_WIDTH || z < 0 || z >= GRID_DEPTH || y - 1 < 0) return false;

		if (grid[y - 1][x][z]) return false; // kolizja z innym klockiem
	}
	return true;
}

void Game::decrementCoordY() {
	for (int i = 0; i < 4; i++) {
		this->currentBlockCoords[i].y -= 1;
		//std::cout << "Decremented coord: y=" << currentBlockCoords[i].y << std::endl;
	}
}

void Game::addTetrominoToGrid() {
	for (int i = 0; i < 4; i++) { // y x z
		//std::cout << "Adding block at: x=" << currentBlockCoords[i].x
		//	<< ", y=" << currentBlockCoords[i].y
		//	<< ", z=" << currentBlockCoords[i].z
		//	<< std::endl;
		this->grid[currentBlockCoords[i].y][currentBlockCoords[i].x][currentBlockCoords[i].z] = true;
		this->texGrid[currentBlockCoords[i].y][currentBlockCoords[i].x][currentBlockCoords[i].z] = this->currentType;
	}
}

bool Game::canMoveByOffset(int dx, int dz) {
	for (int i = 0; i < 4; i++) {
		int x = currentBlockCoords[i].x;
		int y = currentBlockCoords[i].y;
		int z = currentBlockCoords[i].z;

		// Najpierw sprawdzamy, czy obecne wspó³rzêdne s¹ w zakresie (dla bezpieczeñstwa)
		if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT || z < 0 || z >= GRID_DEPTH)
			return false;

		// Teraz sprawdzamy przesuniêcie
		int newX = x + dx;
		int newZ = z + dz;

		if (newX < 0 || newX >= GRID_WIDTH || newZ < 0 || newZ >= GRID_DEPTH)
			return false;

		if (grid[y][newX][newZ]) return false;
	}
	return true;
}

void Game::moveByOffset(int dx, int dz, float angle_x) {
	// Tworzymy wektor ruchu
	glm::vec2 dir(dx, dz);

	// Obracamy go przeciwnie do kierunku obrotu kamery
	float cosA = cos(angle_x);
	float sinA = sin(angle_x);
	glm::vec2 rotatedDir;
	rotatedDir.x = dir.x * cosA - dir.y * sinA;
	rotatedDir.y = dir.x * sinA + dir.y * cosA;

	int finalDx = 0;
	int finalDz = 0;
	if (abs(rotatedDir.x) > abs(rotatedDir.y)) {
		finalDx = rotatedDir.x > 0 ? 1 : -1;
	}
	else {
		finalDz = rotatedDir.y > 0 ? 1 : -1;
	}

	// Sprawdzamy, czy mo¿emy siê ruszyæ w obróconym kierunku
	if (canMoveByOffset(finalDx, finalDz)) {
		// Wykonujemy przesuniêcie
		for (int i = 0; i < 4; i++) {
			currentBlockCoords[i].x += finalDx;
			currentBlockCoords[i].z += finalDz;
		}
	}
}

void Game::moveLeft(float angle_x) {
	moveByOffset(1, 0, angle_x);
}

void Game::moveRight(float angle_x) {
	moveByOffset(-1, 0, angle_x);
}

void Game::moveForward(float angle_x) {
	moveByOffset(0, 1, angle_x);
}

void Game::moveBackward(float angle_x) {
	moveByOffset(0, -1, angle_x);
}

void Game::clearFullLayers(float* stepTime) {

	for (int y = 0; y < GRID_HEIGHT; y++) {
		bool fullLayer = true;

		// Sprawdzamy, czy warstwa jest pe³na
		for (int x = 0; x < GRID_WIDTH && fullLayer; x++) {
			for (int z = 0; z < GRID_DEPTH && fullLayer; z++) {
				if (!grid[y][x][z]) {
					fullLayer = false;
				}
			}
		}

		// Jeœli warstwa pe³na, usuwamy i przesuwamy warstwy w dó³
		if (fullLayer) {

			for (int x = 0; x < GRID_WIDTH; x++) {
				for (int z = 0; z < GRID_DEPTH; z++) {
					TetrominoType type = texGrid[y][x][z];
					if (tetrominoPoints.count(type)) {
						score->addScore(tetrominoPoints[type]);
					}
				}
			}
			*stepTime = *stepTime * 0.95f;

			// Usuñ warstwê y (wyczyœæ)
			for (int x = 0; x < GRID_WIDTH; x++) {
				for (int z = 0; z < GRID_DEPTH; z++) {
					grid[y][x][z] = false;
					texGrid[y][x][z] = TetrominoType::None;
				}
			}

			// Przesuñ wszystkie warstwy powy¿ej y w dó³ o 1
			for (int yy = y + 1; yy < GRID_HEIGHT; yy++) {
				for (int x = 0; x < GRID_WIDTH; x++) {
					for (int z = 0; z < GRID_DEPTH; z++) {
						grid[yy - 1][x][z] = grid[yy][x][z];
						texGrid[yy - 1][x][z] = texGrid[yy][x][z];
					}
				}
			}

			// Wyczyœæ najwy¿sz¹ warstwê (bo zosta³a przesuniêta w dó³)
			int top = GRID_HEIGHT - 1;
			for (int x = 0; x < GRID_WIDTH; x++) {
				for (int z = 0; z < GRID_DEPTH; z++) {
					grid[top][x][z] = false;
					texGrid[top][x][z] = TetrominoType::None;
				}
			}

			// Po usuniêciu warstwy cofamy y, aby ponownie sprawdziæ ten poziom,
			// bo warstwy powy¿ej zosta³y przesuniête w dó³ i mog¹ byæ pe³ne
			y--;
		}
	}
}

std::vector<glm::vec3> Game::rotateBlockCoordsY(const std::vector<glm::vec3>& coords) {
	std::vector<glm::vec3> rotatedCoords;
	glm::vec3 center = coords[1]; // Przyjmij œrodek jako drugi punkt lub oblicz œrodek figury

	for (const auto& point : coords) {
		glm::vec3 local = point - center;

		// Obrót o 90 stopni (mo¿na ogólnie: u¿yj sin/cos)
		glm::vec3 rotated;
		rotated.x = local.z;
		rotated.y = local.y;
		rotated.z = -local.x;

		rotatedCoords.push_back(rotated + center);
	}

	return rotatedCoords;
}

std::vector<glm::vec3> Game::rotateBlockCoordsX(const std::vector<glm::vec3>& coords) {
	std::vector<glm::vec3> rotatedCoords;
	glm::vec3 center = coords[1];

	for (const auto& point : coords) {
		glm::vec3 local = point - center;
		glm::vec3 rotated;
		rotated.x = local.x;
		rotated.y = -local.z;
		rotated.z = local.y;
		rotatedCoords.push_back(rotated + center);
	}
	return rotatedCoords;
}

bool Game::canRotate(std::vector<glm::vec3> rotatedCoords) {

	for (const auto& point : rotatedCoords) {
		int x = point.x;
		int y = point.y;
		int z = point.z;

		if (x < 0 || x >= GRID_WIDTH ||
			y < 0 || y >= GRID_HEIGHT ||
			z < 0 || z >= GRID_DEPTH) {
			return false;
		}

		if (grid[y][x][z]) {
			return false;
		}
	}

	return true;
}

void Game::tryRotateY() {
	std::vector<glm::vec3> rotatedCoords = rotateBlockCoordsY(currentBlockCoords);
	if (canRotate(rotatedCoords)) {
		currentBlockCoords = rotatedCoords;
	}
}

void Game::tryRotateX() {
	std::vector<glm::vec3> rotatedCoords = rotateBlockCoordsX(currentBlockCoords);
	if (canRotate(rotatedCoords)) {
		currentBlockCoords = rotatedCoords;
	}
}

void Game::spawnNextTetromino(float nextTetrominoAngle) {
	glm::mat4 nextTetromino = glm::mat4(1.0f);
	nextTetromino = glm::translate(nextTetromino, glm::vec3(-15.0f, 10.0f, 0.0f));
	nextTetromino = glm::rotate(nextTetromino, glm::radians(nextTetrominoAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	nextTetromino = glm::scale(nextTetromino, glm::vec3(0.5f, 0.5f, 0.5f));

	std::vector<glm::vec3> shape = tetrominoShapes[this->nextType];

	for (const auto& blockPos : shape) {
		this->currentPiece->drawCube(nextTetromino, (*textures)[this->nextType], blockPos);
	}
}

void Game::update(bool timeReady, float* stepTime) {
	if (timeReady) {
		if (!moving) {
			this->currentType = nextType;
			nextType = getRandomTetrominoType();
			moving = true;

			setStartingCoords();
			justSpawned = true;

			if (!canPlace()) {
				std::cout << "Game Over! Cannot place the tetromino." << std::endl;
				exit(0);
			}
		}

		if (justSpawned) {
			justSpawned = false;
		} else if (canMove()) {
			decrementCoordY();
		}
		else {
			addTetrominoToGrid();
			clearFullLayers(stepTime);
			this->moving = false;
			this->currentBlockCoords = std::vector<glm::vec3>(4, glm::vec3(0.0f));
		}
	}
}

void Game::render(glm::mat4 M, float angle_x, float nextTetrominoAngle) {
	this->M = M;
	spawnBoard(M, angle_x);
	spawnStaticBlocks(M);
	spawnCurrentTetromino(M);
	spawnNextTetromino(nextTetrominoAngle);
	this->score->displayScore(this->sp, &this->V, &this->P);
}

void Game::refillBag() {
	bag = {
		TetrominoType::SMASHBOY,
		TetrominoType::Z1,
		TetrominoType::L1,
		TetrominoType::Z2,
		TetrominoType::TEEWEE,
		TetrominoType::L2,
		TetrominoType::HERO
	};

	static std::random_device rd;
	static std::mt19937 g(rd());
	// Shuffle the bag
	std::shuffle(bag.begin(), bag.end(), g);
	currentIndex = 0;
}

TetrominoType Game::getRandomTetrominoType() {
	if (currentIndex >= bag.size()) {
		refillBag();
	}
	return bag[currentIndex++];
}

