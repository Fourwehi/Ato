#pragma once

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <vmath.h>
#include "object.h"
#include "enemy.h"
#include "view.h"
#include "utility.h"
#include "deco.h"
// #include "game.h" // Attempt to remove
#include "RoadKillApp.h" // Added for APP_GRID_SIZE, APP_WIDTH_LIMIT
#include "river.h"
#include "null.h"
#include "resource.h"
#include "light.h"
#include "player.h"

class Generator : public Object {
public:
	enum type { TYPE_NONE, TYPE_ENEMY, TYPE_LOG };
private:
	enum type t;
	bool left;
	float spd;
	float gapMin, gapMax;
	float alarm;

	inline Object* create() {
		Object* o = nullptr;
		switch (t) {
		case TYPE_NONE:
			break;
		case TYPE_ENEMY:
			o = new Car();
			break;
		case TYPE_LOG:
			o = new Log();
			break;
		}
		o->locate(vmath::vec3(left ? -RoadKillApp::APP_WIDTH_LIMIT : RoadKillApp::APP_WIDTH_LIMIT, pos[1], 0.0)); // pos.y -> pos[1]
		o->setXvel(left ? spd : -spd);
		push(o);
		return o;
	}

public:
	inline Generator(enum type t, float y, bool left, float spd, float gapMin, float gapMax)
		: Object(), t(t), left(left), spd(spd), gapMin(gapMin), gapMax(gapMax) {
		locate(vmath::vec3(0.0, y, 0.0));
		for (float dx = frandRange(0.0, gapMax) - RoadKillApp::APP_WIDTH_LIMIT; dx < RoadKillApp::APP_WIDTH_LIMIT; ) {
			create()->move(vmath::vec3(left ? dx : -dx, 0.0, 0.0));
			alarmSet();
			dx += alarm * spd;
		}
		name = "Generator";
	}

	inline void alarmSet() {
		alarm = frandRange(gapMin, gapMax) / spd;
	}

	inline void update() {
		alarm -= 1.0;
		if (alarm <= 0.0) {
			create();
			alarmSet();
		}
		Object::update();
	}
};

class MetaGenerator : public Object {
private:
	inline static float difRate = 0.00005f;
	float target;
	float difficulty;

	inline void placeRoads() {
		NullLimiter* nl = new NullLimiter(target);
		push(nl);

		int lane;
		if (difficulty < 0.25)
			lane = rand() % 2 + 1;
		else if (difficulty < 0.5)
			lane = rand() % 3 + 1;
		else
			lane = rand() % 2 + 2;

		float flane = (float)lane;
		float spdMin = 1.0f + difficulty * 5.0f;
		float spdMax = 2.0f + difficulty * 8.0f;
		float gapMin = 320.0f - difficulty * 200.0f;
		float gapMax = 640.0f - difficulty * 200.0f;

		Road* r = new Road(lane);
		r->locate(vmath::vec3(0.0, target, 0.0));
		nl->push(r);

		const float wlimit = RoadKillApp::APP_WIDTH_LIMIT - RoadKillApp::APP_GRID_SIZE * 1.5f;

		for (float i = 0.0; i < 2.0 * flane; i += 1.0) {
			float y = target + i * RoadKillApp::APP_GRID_SIZE;
			nl->push(new Generator(Generator::TYPE_ENEMY, y, i >= flane, frandRange(spdMin, spdMax), gapMin, gapMax));
			nl->push(new Deco(vmath::vec3(wlimit, y, 0.0), vmath::vec3(0.0, 0.0, 90.0), &Resource::tunnel, &Resource::Tex::tunnel, &Resource::Norm::tunnel));
			nl->push(new Deco(vmath::vec3(-wlimit, y, 0.0), vmath::vec3(0.0, 0.0, -90.0), &Resource::tunnel, &Resource::Tex::tunnel, &Resource::Norm::tunnel));
		}
		target += RoadKillApp::APP_GRID_SIZE * ( flane * 2.0f );
	}

	inline void placeRivers() {
		NullLimiter* nl = new NullLimiter(target);
		push(nl);

		const float wlimit = RoadKillApp::APP_WIDTH_LIMIT - RoadKillApp::APP_GRID_SIZE * 1.5f;

		for (int repeat = rand() % 3 + 1; repeat > 0; repeat--) {
			bool left = rand() % 2 ? true : false;

			float spdMin = 0.5f + difficulty * 3.0f;
			float spdMax = 0.5f + difficulty * 6.0f;
			float gapMin = 200.0f + difficulty * 100.0f;
			float gapMax = 320.0f + difficulty * 300.0f;

			nl->push(new Water(target));
			nl->push(new Generator(Generator::TYPE_LOG, target, left, frandRange(spdMin, spdMax), gapMin, gapMax));
			nl->push(new Deco(vmath::vec3(-wlimit, target, 0.0), vmath::vec3(0.0, 0.0, 180.0), &Resource::drain, &Resource::Tex::tunnel, &Resource::Norm::tunnel));
			nl->push(new Deco(vmath::vec3(wlimit, target, 0.0), vmath::vec3(0.0, 0.0, 0.0), &Resource::drain, &Resource::Tex::tunnel, &Resource::Norm::tunnel));
			target += RoadKillApp::APP_GRID_SIZE;
		}
	}

	inline void placeSafezone() {
		NullLimiter* nl = new NullLimiter(target);
		push(nl);

		int repeat = rand() % 3 + 1;

		nl->push(new Deco(vmath::vec3(0.0, target, 0.0), &Resource::grass[repeat - 1], &Resource::Tex::grass, &Resource::Norm::grass));

		for (int i = 0; i < repeat; i++)
			placeTrees(target + RoadKillApp::APP_GRID_SIZE * i);

		for (int i = 0; i < repeat + 1; i++) {
			if (frand() < 0.5f) {
				int limit = (int)( RoadKillApp::APP_WIDTH_LIMIT / RoadKillApp::APP_GRID_SIZE );
				int ind = rand() % ( 2 * limit - 1 ) - limit;
				nl->push(new StreetLight(vmath::vec3(( (float)ind + 0.5f ) * RoadKillApp::APP_GRID_SIZE, target + RoadKillApp::APP_GRID_SIZE * ( (float)i - 0.5f ), 0.0)));
			}
		}

		target += RoadKillApp::APP_GRID_SIZE * repeat;
	}

public:
	inline MetaGenerator() : Object(), difficulty(0.0f), target(RoadKillApp::APP_GRID_SIZE * 4.0f) { // Used APP_GRID_SIZE
		name = "Metagenerator";
	}

	inline void update() {
		difficulty = fminf(View::getY() * difRate, 1.0f);
		if (View::getY() + View::getZfar() > target) {
			if (frand() > 0.2)
				placeRoads();
			else
				placeRivers();
			placeSafezone();
		}
		Object::update();
	}

#include "Player.h" // Ensure Player.h is included

// ... (other includes and class code) ...

// Inside MetaGenerator class
	inline void placeTrees(float y) {
		NullLimiter* nl = new NullLimiter(y);
		push(nl);
		// float playerXLimitPlaceholder = RoadKillApp::APP_WIDTH_LIMIT / 2.0f; // Removed placeholder
		nl->push(new Tree(Player::getXlimit() + RoadKillApp::APP_GRID_SIZE, y)); // Restored Player::getXlimit()
		nl->push(new Tree(-Player::getXlimit() - RoadKillApp::APP_GRID_SIZE, y)); // Restored Player::getXlimit()

		if (frand() > 0.6 + 0.4 * difficulty)
			return;

		int num = rand() % 2 + 1;
		int plimit = (int)( Player::getXlimit() / RoadKillApp::APP_GRID_SIZE ); // Restored Player::getXlimit()
		int limit = (int)( RoadKillApp::APP_WIDTH_LIMIT / RoadKillApp::APP_GRID_SIZE );
		int location = rand() % ( plimit * 2 + 2 - num ) - plimit;

		for (int i = 0; i < num; i++) {
			nl->push(new Tree((float)( location + i ) * RoadKillApp::APP_GRID_SIZE, y));
		}
	}
};
