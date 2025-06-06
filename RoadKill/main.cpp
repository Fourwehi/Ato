/*
// BetterCarryingEachOther 2016.03.11
// CSED451 Assignment
// THIS FILE IS NO LONGER THE MAIN ENTRY POINT. RoadKillApp.cpp (DECLARE_MAIN) is.
//

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "key.h"
#include "game.h"
#include "view.h"
#include "shader.h"
#include "model.h"
#include "resource.h"

void update(int cnt) {
	glutTimerFunc(Game::getFrameInterval(), update, cnt + 1);

	Game::update();
	Game::draw();
	Key::keyUpdate();
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize((int)View::getWidth(), (int)View::getHeight());
	
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("BetterCarryingEachOther Assn4");

	glewExperimental = GL_TRUE;
	glewInit();

	Shader::init();
	Model::init();
	Resource::init();

	Game::init();
	
	glutReshapeFunc(Game::reshape);
	glutDisplayFunc(Game::draw);
	
	glutTimerFunc(Game::getFrameInterval(), update, 0);

	glutKeyboardFunc(Key::keyPressed);
	glutKeyboardUpFunc(Key::keyReleased);

	glutMainLoop();
}
*/