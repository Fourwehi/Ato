
#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>

#include <vmath.h>
#include "object.h"
#include "texture.h"
#include "view.h"
#include "deco.h"
#include "shader.h"
#include "resource.h"
// #include "game.h" // Attempt to remove
#include "RoadKillApp.h" // Added for APP_GRID_SIZE, APP_WIDTH_LIMIT

class Enemy : public Object {
protected:
	float dist;

public:
	inline Enemy(float cbWidth, float cbHeight, float cbOffX, float cbOffY)
		: Object(cbWidth, cbHeight, cbOffX, cbOffY), dist(0.0f) {
		cat = OBJ_ENEMY;
	}

	inline void update() {
		Object::update();
		dist += vmath::length(vel);

		if (pos[0] > RoadKillApp::APP_WIDTH_LIMIT || pos[0] < -RoadKillApp::APP_WIDTH_LIMIT) // pos.x -> pos[0]
			expire();
	}
};

class Car : public Enemy {
private:
	Texture* tex;

public:
	inline Car() : Enemy(70.0f, 30.0f, 35.0f, 15.0f) {
		tex = &Resource::Tex::carWhite;
		name = "Car";
	}

	inline void draw() const {
		tex->bind();
		Resource::Norm::car.bind();

		Shader::push();
		Shader::translate(pos);
		Shader::translate(vmath::vec3(0.0, 0.0, 0.0));
		if (vel[0] > 0) // vel.x -> vel[0]
			Shader::rotateZ(180.0);
		Shader::apply();
		Resource::car.draw();

		const float carWidth = 22.0;
		const float carWidth = 22.0;
		const float wheelFront = -30.0;
		const float wheelBack = 28.0;
		const float wheelHeight = 10.0;
		// const float wheelAngle = dist / wheelHeight / DegreesToRadians; // old way
		const float wheelAngle = dist / wheelHeight; // Assuming this is now in radians for new shader system

		Resource::Tex::wheel.bind();
		Resource::Norm::flat.bind();

		Shader::push();
		Shader::translate(vmath::vec3(wheelFront, carWidth, wheelHeight));
		Shader::rotateX(-90.0); // These will be replaced with vmath::rotate(vmath::radians(angle_deg), axis) later
		Shader::rotateZ(-wheelAngle); // Assuming wheelAngle is radians now, Shader::rotateZ needs to handle radians
		Shader::apply();
		Resource::wheel.draw();
		Shader::pop();

		Shader::push();
		Shader::translate(vmath::vec3(wheelFront, -carWidth, wheelHeight));
		Shader::rotateX(90.0);
		Shader::rotateZ(wheelAngle);
		Shader::apply();
		Resource::wheel.draw();
		Shader::pop();

		Shader::push();
		Shader::translate(vmath::vec3(wheelBack, carWidth, wheelHeight));
		Shader::rotateX(-90.0);
		Shader::rotateZ(-wheelAngle);
		Shader::apply();
		Resource::wheel.draw();
		Shader::pop();

		Shader::push();
		Shader::translate(vmath::vec3(wheelBack, -carWidth, wheelHeight));
		Shader::rotateX(90.0);
		Shader::rotateZ(wheelAngle);
		Shader::apply();
		Resource::wheel.draw();
		Shader::pop();

		Object::draw();
		Shader::pop();
	}
};
