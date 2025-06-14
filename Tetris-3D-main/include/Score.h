#pragma once

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
#include <thread>
#include <chrono>
#include "model.h"
#include "objParser.h"
#include <memory>

class Score {
public:
	Score();
	~Score();
	void addScore(int value);
	void displayScore(ShaderProgram* sp, glm::mat4* V, glm::mat4* P);
	void loadTextures();
private:
	int score;
	std::map<std::string, GLuint> textures;
	std::vector<std::shared_ptr<Model>> models;
	std::vector<std::string> fileNames = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	std::vector<int> cutScore(int score);
};