#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

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
#include "Game.h"
#include <map>
#include <iostream>
#include <thread>
#include <chrono>
#include "model.h"
#include "objParser.h"
#include <memory>
#include "Score.h"

float speed_x=0;
float angle_x = 0;
float aspectRatio=1;

float startingStepTime = 1.0f;
float fastStepTime = 0.3f;
float stepTime = startingStepTime;
float currentStepTime = startingStepTime;

float nextTetrominoSpeed = 8 * PI;
float nextTetrominoAngle = 0;

ShaderProgram *sp;

glm::mat4 game;
glm::mat4 V;
glm::mat4 P;
glm::mat4 staticMatrix;
Game *gameInstance;

std::map<TetrominoType, GLuint> textures;

std::map<TetrominoType, GLuint> getTextures() {

	std::vector<TetrominoType> types = {
		TetrominoType::HERO, TetrominoType::L1, TetrominoType::L2,
		TetrominoType::Z1, TetrominoType::Z2, TetrominoType::TEEWEE, TetrominoType::SMASHBOY
	};

	std::cout << "Loading textures..." << std::endl;
	for (TetrominoType type : types) {
		std::string fileName = "textures2/" + Tetromino::tetrominoTypeToString(type) + ".png";
		GLuint tex = readTexture(fileName.c_str());
		textures[type] = tex;
	}
	std::cout << "Textures loaded." << std::endl;
	
	return textures;
}

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_LEFT) speed_x=PI/2;
        if (key==GLFW_KEY_RIGHT) speed_x=-PI/2;
		if (key == GLFW_KEY_A) gameInstance->moveLeft(angle_x);
		if (key == GLFW_KEY_D) gameInstance->moveRight(angle_x);
		if (key == GLFW_KEY_W) gameInstance->moveForward(angle_x);
		if (key == GLFW_KEY_S) gameInstance->moveBackward(angle_x);
		if (key == GLFW_KEY_Q) gameInstance->tryRotateY();
		if (key == GLFW_KEY_E) gameInstance->tryRotateX();
		if (key == GLFW_KEY_DOWN || key == GLFW_KEY_SPACE) {
			stepTime = std::min(fastStepTime, currentStepTime);
		}
		if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_LEFT) speed_x=0;
        if (key==GLFW_KEY_RIGHT) speed_x=0;
		if (key == GLFW_KEY_DOWN || key == GLFW_KEY_SPACE) {
			stepTime = currentStepTime;
		}
    }
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);

	P = glm::perspective(
		glm::radians(50.0f), //Kąt widzenia w radianach
		aspectRatio,         //Stosunek wysokości do szerokości
		1.0f,                //Minimalna odległość rysowania
		50.0f                //Maksymalna odległość rysowania
	);
}

void initOpenGLProgram(GLFWwindow* window) {
	glClearColor(0.07, 0.10, 0.15, 1.0);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);

	sp = new ShaderProgram("shaders/v_simplest.glsl",NULL,"shaders/f_simplest.glsl");
	textures = getTextures();

	V = glm::lookAt(
		glm::vec3(0.0f, 20.0f, -30.0f), //Położenie kamery
		glm::vec3(0.0f, 12.0f, 0.0f), //Na co patrzy kamera
		glm::vec3(0.0f, 1.0f, 0.0f) //Wektor wskazujący kierunek "góry" kamery
	);

	gameInstance = new Game(window, sp, game, &V, &P, &textures);


}

void freeOpenGLProgram(GLFWwindow* window) {
	delete sp;
	sp = nullptr;
	delete gameInstance;
	gameInstance = nullptr;


	for (const auto& pair : textures) {
		glDeleteTextures(1, &pair.second);
	}
	textures.clear();
}

void drawScene(GLFWwindow* window, float angle_x, float nextTetrominoAngle) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	game = glm::mat4(1.0f);
	game = glm::rotate(game, angle_x, glm::vec3(0.0f, 1.0f, 0.0f));
	angle_x = fmod(angle_x, 2 * PI);
	if (angle_x < 0) {
		angle_x = 2 * PI - abs(angle_x);
	}
	//printf("%f\n", angle_x);
	gameInstance->render(game, angle_x, nextTetrominoAngle);

	glfwSwapBuffers(window);
}


int main(void)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary);
	window = glfwCreateWindow(mode->width, mode->height, "OpenGL", primary, NULL); //  primary, NULL - ekran

	if (!window)
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);  // << pobierz rozmiar okna
	windowResizeCallback(window, width, height);      // << wymuś przeliczenie macierzy

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window);


	double previousTime = glfwGetTime();
	float deltaTime = 0.0f;
	float accumulator = 0.0f; // ile czasu minęło łącznie

	glfwSetTime(0);
	while (!glfwWindowShouldClose(window))
	{
		double currentTime = glfwGetTime();
		deltaTime = static_cast<float>(currentTime - previousTime);
		previousTime = currentTime;

		accumulator += deltaTime;

        angle_x += speed_x * deltaTime;
		nextTetrominoAngle += nextTetrominoSpeed * deltaTime;
		//std::cout << "stepTime: " << stepTime << std::endl;

		if (accumulator >= stepTime) {
			accumulator -= stepTime;
			gameInstance->update(true, &currentStepTime);
		}
		else {
			gameInstance->update(false, &currentStepTime);
		}

		drawScene(window, angle_x, nextTetrominoAngle);
		glfwPollEvents();
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}