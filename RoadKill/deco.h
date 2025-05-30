#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <vmath.h>
#include "utility.h"
#include "view.h"
// #include "game.h" // Attempt to remove
#include "RoadKillApp.h" // Added for APP_GRID_SIZE, APP_WIDTH_LIMIT
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
		Shader::translate(vmath::vec3(0.0, ( lane - 0.5 ) * RoadKillApp::APP_GRID_SIZE, 0.0));
		Shader::apply();
		Resource::Tex::road.bind();
		Resource::Norm::road.bind();
		Resource::road[lane - 1].draw();
		Object::draw();
		Shader::pop();
	}

	inline void update() {
		Object::update();
		if (View::getY() > pos[1] + View::getZfar()) { // pos.y -> pos[1]
			expire();
		}
	}
};

class Env : public Object {
public:
	inline Env(float x, float y, float cbWidth, float cbHeight, float cbOffX, float cbOffY)
		: Object(cbWidth, cbHeight, cbOffX, cbOffY) {
		locate(vmath::vec3(x, y, 0.0));
	}

	inline Env(vmath::vec3 pos) : Object(0.0, 0.0, 0.0, 0.0) {
		locate(pos);
	}
};

class Tree : public Env {
private:
	float scale, rotation;

public:
	inline Tree(float x, float y)
		: Env(x, y, RoadKillApp::APP_GRID_SIZE, RoadKillApp::APP_GRID_SIZE, RoadKillApp::APP_GRID_SIZE * 0.5f, RoadKillApp::APP_GRID_SIZE * 0.5f) { // Added 'f' suffix
		scale = frandRange(1.0, 1.5);
		rotation = frandRange(0.0, 360.0);
		cat = OBJ_RIGID;
		name = "Tree";
	}

	inline void draw() const {
		Shader::push();
		Shader::translate(pos);
		Shader::scale(vmath::vec3(scale, scale, scale));
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
	vmath::vec3 orient;
	vmath::vec2 uvshift;

public:
	inline Deco(vmath::vec3 pos, Model* model, Texture* texture, Texture* normalmap)
		: model(model), texture(texture), normalmap(normalmap), uvshift(vmath::vec2(0.0f, 0.0f)) { // Assuming 0.0 needs to be vec2
		locate(pos);
		name = "Deco";
	}

	inline Deco(vmath::vec3 pos, vmath::vec3 orient, Model* model, Texture* texture, Texture* normalmap)
		: Deco(pos, model, texture, normalmap) {
		this->orient = orient;
	}

	inline void draw() const {
		Shader::push();
		Shader::translate(pos);
		Shader::rotateX(orient[0]); // orient.x -> orient[0]
		Shader::rotateY(orient[1]); // orient.y -> orient[1]
		Shader::rotateZ(orient[2]); // orient.z -> orient[2]
		Shader::shift(uvshift);
		Shader::apply();
		texture->bind();
		normalmap->bind();
		model->draw();
		Object::draw();
		Shader::pop();
	}

	inline void shift(vmath::vec2 uv) {
		uvshift = uv;
	}
};
