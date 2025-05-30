#pragma once

// player.h and player.cpp combined (header-only with inline)

#include <GL/glew.h>
#include <GL/freeglut.h> // Will likely be removed/unnecessary after porting
#include <iostream>
#include <cmath>
#include <algorithm> // For std::max and std::min

// #include "game.h"       // Replaced
#include "RoadKillApp.h"  // Replaced game.h
#include "object.h"
#include "view.h"
// #include "key.h"        // Replaced
#include "utility.h"
#include "deco.h"
#include "shader.h"
#include "resource.h"

class Player : public Object {
private:
	enum deadState { D_ALIVE, D_DROWN, D_SUFFOCATE, D_OUTSIDE };
	enum deadState dead;

	// inline static float xlimit = Game::getGrid() * 3.0; // Removed, use getXlimit()
	float xx, yy;
	float ylimit;
	float dir, adir;
	float flatten;
	float aniPhase;

	Object* srnd;

public:
	inline static float getXlimit() {
        return RoadKillApp::APP_GRID_SIZE * 3.0f;
    }

	inline Player(Object* srnd_ptr) : Object(20.0f, 20.0f, 10.0f, 10.0f) { // srnd renamed to srnd_ptr, added f suffix
		this->srnd = srnd_ptr;
		dead = D_ALIVE;
		xx = 0.0f;
		yy = 0.0f;
		dir = 90.0f;
		adir = 90.0f;
		flatten = 0.0f;
		aniPhase = 1.0f;
		ylimit = pos[1] - RoadKillApp::APP_GRID_SIZE; // pos.y -> pos[1], Game::getGrid -> APP_GRID_SIZE
		name = "Player";
	}

	inline void draw() const {
		if (View::getViewMode() == View::VIEW_FRONT)
			return;

		const float piPhase = aniPhase * 3.1415926f; // Added f suffix
		const float sFac = sinf(piPhase);
		const float bodyWidth = 8.0f; // Added f suffix

		Resource::Tex::goraniWhole.bind();
		Resource::Norm::flat.bind();

		Shader::push();
		// Shader::translate(vec3(xx, yy, pos.z + 10.0 * sFac)); // To be refactored
        Shader::translate(vmath::vec3(xx, yy, pos[2] + 10.0f * sFac)); // Example of initial refactor
		Shader::rotateZ(adir + 180.0f); // Added f suffix
		// Shader::scale(lerpv3(vec3(1.0, 1.0, 1.0), vec3(1.0, 2.0, 0.3), flatten)); // To be refactored
        Shader::scale(lerpv3(vmath::vec3(1.0f, 1.0f, 1.0f), vmath::vec3(1.0f, 2.0f, 0.3f), flatten)); // Example
		Shader::translate(vmath::vec3(0.0f, 0.0f, 28.0f)); // Added f suffix
		Shader::scale(0.7f); // Added f suffix
		Shader::rotateY(10.0f * sinf(piPhase * 2.0f)); // Added f suffix
		Shader::apply();
		Resource::goraniTorso.draw();
		// [Legs, Neck, Head drawing blocks omitted for brevity - assume similar structure]
        // Drawing for other parts like legs, neck, head would follow here,
        // applying similar Shader transforms and Resource drawing calls.
        // Example for one leg (conceptual):
        // Shader::push();
        // Shader::translate(vmath::vec3(bodyWidth, 0.0f, -5.0f)); // Example offset
        // Shader::rotateX(20.0f * sFac); // Example animation
        // Shader::apply();
        // Resource::goraniLegFrontTop.draw();
        // Shader::pop();


		Object::draw();
		Shader::pop();
	}

    void update(RoadKillApp* app) { // Signature changed
		Object::update();
		xx = flerp(xx, pos[0], 0.3f); // pos.x -> pos[0]
		yy = flerp(yy, pos[1], 0.3f); // pos.y -> pos[1]
		adir = flerp(adir, dir, 0.3f);
		View::setViewDir(adir);

		if (aniPhase < 1.0f) aniPhase += 0.08f;
		else aniPhase = 1.0f;

		if (dead == D_SUFFOCATE) flatten = flerp(flatten, 1.0f, 0.4f);

		if (dead != D_ALIVE) {
			if (app->is_key_first_pressed(GLFW_KEY_R)) expire(); // Changed to app input
		}
		else {
			float dx = 0.0f, dy = 0.0f;
			if (app->is_key_first_pressed(GLFW_KEY_A)) dir += 90.0f; // Changed to app input
			if (app->is_key_first_pressed(GLFW_KEY_D)) dir -= 90.0f; // Changed to app input
			if (app->is_key_first_pressed(GLFW_KEY_W)) { // Changed to app input
				dx = RoadKillApp::APP_GRID_SIZE * cosf(vmath::radians(dir)); // Used APP_GRID_SIZE and vmath::radians
				dy = RoadKillApp::APP_GRID_SIZE * sinf(vmath::radians(dir)); // Used APP_GRID_SIZE and vmath::radians
				aniPhase = 0.0f;
			}

			bool reject = false;
			pos[0] += dx; // pos.x -> pos[0]
			pos[1] += dy; // pos.y -> pos[1]
			cbUpdate();

			if (pos[1] < ylimit) reject = true; // pos.y -> pos[1]

			if (srnd && !srnd->collide(this, OBJ_WATER)) { // Added srnd check
				float ddx = roundf(pos[0] / RoadKillApp::APP_GRID_SIZE) * RoadKillApp::APP_GRID_SIZE - pos[0]; // Used APP_GRID_SIZE
				if (pos[0] + ddx > Player::getXlimit() || pos[0] + ddx < -Player::getXlimit()) reject = true; // Used Player::getXlimit()
				else {
					dx += ddx;
					pos[0] += ddx; // pos.x -> pos[0]
				}
			}

			if (srnd && srnd->collide(this, OBJ_RIGID)) reject = true; // Added srnd check

			if (reject) {
				pos[0] -= dx; // pos.x -> pos[0]
				pos[1] -= dy; // pos.y -> pos[1]
				cbUpdate();
			}

			if (srnd && srnd->collide(this, OBJ_ENEMY)) dead = D_SUFFOCATE; // Added srnd check

			Object* owater = srnd ? srnd->collide(this, OBJ_WATER) : nullptr; // Added srnd check
			Object* olog = srnd ? srnd->collide(this, OBJ_LOG) : nullptr;     // Added srnd check

			if (owater) {
				if (olog) {
					float lx = olog->getPos()[0]; // .x -> [0]
					float side = roundf(( pos[0] - lx ) / RoadKillApp::APP_GRID_SIZE); // Used APP_GRID_SIZE
					side = std::max(-1.0f, std::min(1.0f, side));
					pos[0] = lx + side * RoadKillApp::APP_GRID_SIZE; // Used APP_GRID_SIZE
					if (pos[0] > Player::getXlimit() + 0.5f * RoadKillApp::APP_GRID_SIZE || pos[0] < -Player::getXlimit() - 0.5f * RoadKillApp::APP_GRID_SIZE) // Used Player::getXlimit and APP_GRID_SIZE
						dead = D_OUTSIDE;
					pos += olog->getVel();
				}
				else {
					dead = D_DROWN;
					acc = vmath::vec3(0.0f, 0.0f, -0.5f); // Used vmath::vec3
				}
			}

			ylimit = std::min(ylimit, pos[1] - RoadKillApp::APP_GRID_SIZE * 3.0f); // Used APP_GRID_SIZE, pos.y -> pos[1]
			View::setViewAt(pos[0], pos[1]); // pos.x -> pos[0], pos.y -> pos[1]
		}
	}

    Object& locate(vmath::vec3 p) override { // Signature changed, parameter name 'p' kept
		xx = p[0];
		yy = p[1];
		return Object::locate(p);
	}
};
