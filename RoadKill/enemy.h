
#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>

#include "object.h"
#include "texture.h"
#include "view.h"
#include "deco.h"
#include "shader.h"
#include "resource.h"
#include "game.h"

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
		dist += length(vel);

		if (pos.x > Game::getWidthLimit() || pos.x < -Game::getWidthLimit())
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
		Shader::translate(vec3(0.0, 0.0, 0.0));
		if (vel.x > 0)
			Shader::rotateZ(180.0);
		Shader::apply();
		Resource::car.draw();

		const float carWidth = 22.0;
		const float wheelFront = -30.0;
		const float wheelBack = 28.0;
		const float wheelHeight = 10.0;
		const float wheelAngle = dist / wheelHeight / DegreesToRadians;

		Resource::Tex::wheel.bind();
		Resource::Norm::flat.bind();

		Shader::push();
		Shader::translate(vec3(wheelFront, carWidth, wheelHeight));
		Shader::rotateX(-90.0);
		Shader::rotateZ(-wheelAngle);
		Shader::apply();
		Resource::wheel.draw();
		Shader::pop();

		Shader::push();
		Shader::translate(vec3(wheelFront, -carWidth, wheelHeight));
		Shader::rotateX(90.0);
		Shader::rotateZ(wheelAngle);
		Shader::apply();
		Resource::wheel.draw();
		Shader::pop();

		Shader::push();
		Shader::translate(vec3(wheelBack, carWidth, wheelHeight));
		Shader::rotateX(-90.0);
		Shader::rotateZ(-wheelAngle);
		Shader::apply();
		Resource::wheel.draw();
		Shader::pop();

		Shader::push();
		Shader::translate(vec3(wheelBack, -carWidth, wheelHeight));
		Shader::rotateX(90.0);
		Shader::rotateZ(wheelAngle);
		Shader::apply();
		Resource::wheel.draw();
		Shader::pop();

		Object::draw();
		Shader::pop();
	}
};
