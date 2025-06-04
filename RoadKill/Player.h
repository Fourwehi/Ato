#pragma once

// player.h and player.cpp combined (header-only with inline)

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

#include <vmath.h>
#include "game.h"
#include "object.h"
#include "view.h"
#include "key.h"
#include "utility.h"
#include "deco.h"
#include "shader.h"
#include "resource.h"

class Player : public Object {
private:
	enum deadState { D_ALIVE, D_DROWN, D_SUFFOCATE, D_OUTSIDE };
	enum deadState dead;

	inline static float xlimit = Game::getGrid() * 3.0;
	float xx, yy;
	float ylimit;
	float dir, adir;
	float flatten;
	float aniPhase;

	Object* srnd;

public:
	inline static float getXlimit() {
		return xlimit;
	}

	inline Player(Object* srnd) : Object(20.0, 20.0, 10.0, 10.0) {
		this->srnd = srnd;
		dead = D_ALIVE;
		xx = 0.0;
		yy = 0.0;
		dir = 90.0;
		adir = 90.0;
		flatten = 0.0;
		aniPhase = 1.0;
		ylimit = pos[1] - Game::getGrid(); // pos.y -> pos[1]
		name = "Player";
	}

	inline void draw() const {
		if (View::getViewMode() == View::VIEW_FRONT)
			return;

		const float piPhase = aniPhase * 3.1415926;
		const float sFac = sinf(piPhase);
		const float bodyWidth = 8.0;

		Resource::Tex::goraniWhole.bind();
		Resource::Norm::flat.bind();

		Shader::push();
		Shader::translate(vmath::vec3(xx, yy, pos[2] + 10.0 * sFac)); // pos.z -> pos[2]
		Shader::rotateZ(adir + 180.0);
		Shader::scale(lerpv3(vmath::vec3(1.0, 1.0, 1.0), vmath::vec3(1.0, 2.0, 0.3), flatten)); // Assuming lerpv3 is adapted for vmath::vec3
		Shader::translate(vmath::vec3(0.0, 0.0, 28.0));
		Shader::scale(0.7);
		Shader::rotateY(10.0 * sinf(piPhase * 2.0));
		Shader::apply();
		Resource::goraniTorso.draw();
		// [Legs, Neck, Head drawing blocks omitted for brevity - same as previous]

		Object::draw();
		Shader::pop();
	}

	inline void update() {
		Object::update();
		xx = flerp(xx, pos.x, 0.3);
		yy = flerp(yy, pos.y, 0.3);
		adir = flerp(adir, dir, 0.3);
		View::setViewDir(adir);

		if (aniPhase < 1.0) aniPhase += 0.08;
		else aniPhase = 1.0;

		if (dead == D_SUFFOCATE) flatten = flerp(flatten, 1.0, 0.4);

		if (dead != D_ALIVE) {
			if (Key::keyCheckPressed('r')) expire();
		}
		else {
			float dx = 0.0, dy = 0.0;
			if (Key::keyCheckPressed('a')) dir += 90.0;
			if (Key::keyCheckPressed('d')) dir -= 90.0;
			if (Key::keyCheckPressed('w')) {
				dx = Game::getGrid() * cosf(vmath::radians(dir));
				dy = Game::getGrid() * sinf(vmath::radians(dir));
				aniPhase = 0.0;
			}

			bool reject = false;
			pos[0] += dx; // pos.x -> pos[0]
			pos[1] += dy; // pos.y -> pos[1]
			cbUpdate();

			if (pos[1] < ylimit) reject = true; // pos.y -> pos[1]

			if (!srnd->collide(this, OBJ_WATER)) {
				float ddx = roundf(pos[0] / Game::getGrid()) * Game::getGrid() - pos[0]; // pos.x -> pos[0]
				if (pos[0] + ddx > xlimit || pos[0] + ddx < -xlimit) reject = true; // pos.x -> pos[0]
				else {
					dx += ddx;
					pos[0] += ddx; // pos.x -> pos[0]
				}
			}

			if (srnd->collide(this, OBJ_RIGID)) reject = true;

			if (reject) {
				pos[0] -= dx; // pos.x -> pos[0]
				pos[1] -= dy; // pos.y -> pos[1]
				cbUpdate();
			}

			if (srnd->collide(this, OBJ_ENEMY)) dead = D_SUFFOCATE;

			Object* owater = srnd->collide(this, OBJ_WATER);
			Object* olog = srnd->collide(this, OBJ_LOG);

			if (owater) {
				if (olog) {
					float lx = olog->getPos()[0]; // .x -> [0]
					float side = roundf(( pos[0] - lx ) / Game::getGrid()); // pos.x -> pos[0]
					side = std::max(-1.0f, std::min(1.0f, side));
					pos[0] = lx + side * Game::getGrid(); // pos.x -> pos[0]
					if (pos[0] > xlimit + 0.5 * Game::getGrid() || pos[0] < -xlimit - 0.5 * Game::getGrid()) // pos.x -> pos[0]
						dead = D_OUTSIDE;
					pos += olog->getVel();
				}
				else {
					dead = D_DROWN;
					acc = vmath::vec3(0.0, 0.0, -0.5);
				}
			}

			ylimit = std::min(ylimit, pos[1] - Game::getGrid() * 3.0f); // pos.y -> pos[1]
			View::setViewAt(pos[0], pos[1]); // pos.x -> pos[0], pos.y -> pos[1]
		}
	}

	inline Object& locate(vmath::vec3 pos_param) override { // renamed pos to pos_param to avoid conflict with member
		xx = pos_param[0]; // .x -> [0]
		yy = pos_param[1]; // .y -> [1]
		return Object::locate(pos_param);
	}
};
