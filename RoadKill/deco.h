#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include "utility.h"
#include "view.h"
#include "game.h"
#include "resource.h"
#include "shader.h"
#include "debug.h"

#include "object.h"
#include "texture.h"
#include "model.h"

class Road : public Object {
private:
	int lane;
public:
	inline Road(int lane) : Object(0.0, 0.0, 0.0, 0.0), lane(lane) {
		name = "Road";
	}

	inline void draw() const {
		Shader::push();
		Shader::translate(pos);
		Shader::translate(vec3(0.0, ( lane - 0.5 ) * Game::getGrid(), 0.0));
		Shader::apply();
		Resource::Tex::road.bind();
		Resource::Norm::road.bind();
		Resource::road[lane - 1].draw();
		Object::draw();
		Shader::pop();
	}

	inline void update() {
		Object::update();
		if (View::getY() > pos.y + View::getZfar()) {
			expire();
		}
	}
};

class Env : public Object {
public:
	inline Env(float x, float y, float cbWidth, float cbHeight, float cbOffX, float cbOffY)
		: Object(cbWidth, cbHeight, cbOffX, cbOffY) {
		locate(vec3(x, y, 0.0));
	}

	inline Env(vec3 pos) : Object(0.0, 0.0, 0.0, 0.0) {
		locate(pos);
	}
};

class Tree : public Env {
private:
	float scale, rotation;

public:
	inline Tree(float x, float y)
		: Env(x, y, Game::getGrid(), Game::getGrid(), Game::getGrid() * 0.5, Game::getGrid() * 0.5) {
		scale = frandRange(1.0, 1.5);
		rotation = frandRange(0.0, 360.0);
		cat = OBJ_RIGID;
		name = "Tree";
	}

	inline void draw() const {
		Shader::push();
		Shader::translate(pos);
		Shader::scale(vec3(scale, scale, scale));
		Shader::rotateZ(rotation);
		Shader::apply();
		Resource::Norm::tree.bind();
		Resource::Tex::tree.bind();
		Resource::tree.draw();
		Object::draw();
		Shader::pop();
	}
};

class Deco : public Object {
private:
	Model* model;
	Texture* texture;
	Texture* normalmap;
	vec3 orient;
	vec2 uvshift;

public:
	inline Deco(vec3 pos, Model* model, Texture* texture, Texture* normalmap)
		: model(model), texture(texture), normalmap(normalmap), uvshift(0.0) {
		locate(pos);
		name = "Deco";
	}

	inline Deco(vec3 pos, vec3 orient, Model* model, Texture* texture, Texture* normalmap)
		: Deco(pos, model, texture, normalmap) {
		this->orient = orient;
	}

	inline void draw() const {
		Shader::push();
		Shader::translate(pos);
		Shader::rotateX(orient.x);
		Shader::rotateY(orient.y);
		Shader::rotateZ(orient.z);
		Shader::shift(uvshift);
		Shader::apply();
		texture->bind();
		normalmap->bind();
		model->draw();
		Object::draw();
		Shader::pop();
	}

	inline void shift(vec2 uv) {
		uvshift = uv;
	}
};
