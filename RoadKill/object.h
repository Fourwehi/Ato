#pragma once

// object.h and object.cpp combined as a single inline header

#include <list>
#include <iostream>
#include <vmath.h>
#include "model.h"
#include "view.h"
#include "game.h"
#include "debug.h"

using namespace std;

class Object {
protected:
	inline static int count = 0;

	const char* name;

public:
	enum Category { OBJ_ANY, OBJ_NONE, OBJ_RIGID, OBJ_ENEMY, OBJ_WATER, OBJ_LOG };
protected:
	enum Category cat;
	float cbWidth, cbHeight, cbOffX, cbOffY;
	vmath::vec3 pos, vel, acc;

	bool expired;

	class CBox {
	private:
		float x1, y1, x2, y2;

	public:
		inline bool check(CBox* other) {
			return this->x1 <= other->x2 && this->x2 >= other->x1 &&
				this->y1 <= other->y2 && this->y2 >= other->y1;
		}

		inline bool inclusion(float x, float y) {
			return x > x1 && x < x2 && y > y1 && y < y2;
		}

		inline void locate(float x, float y, float w, float h, float ofx, float ofy) {
			x1 = x - ofx;
			y1 = y - ofy;
			x2 = x + w - ofx;
			y2 = y + h - ofy;
		}
	} cb;

	inline void cbUpdate() {
		cb.locate(pos[0], pos[1], cbWidth, cbHeight, cbOffX, cbOffY);
	}

	Model* mod;
	list<Object*> children;

public:
	inline static void countReset() { count = 0; }
	inline static int countGet() { return count; }

	inline Object(float cbWidth, float cbHeight, float cbOffX, float cbOffY)
		: cbWidth(cbWidth), cbHeight(cbHeight), cbOffX(cbOffX), cbOffY(cbOffY), cat(OBJ_NONE), expired(false), name("Undefined") {
	}

	inline Object() : Object(0.0, 0.0, 0.0, 0.0) {}
	inline virtual ~Object() {}

	inline virtual void echo() { cout << name << endl; }

	inline virtual void draw() const {
		for (auto itor = children.begin(); itor != children.end(); ++itor)
			( *itor )->draw();
	}

	inline virtual void update() {
		count++;
		vel += acc;
		pos += vel;
		cbUpdate();

		for (auto itor = children.begin(); itor != children.end(); ++itor)
			( *itor )->update();

		auto itor = children.begin();
		while (itor != children.end()) {
			auto citor = itor++;
			if (( *citor )->isExpired()) {
				delete* citor;
				children.erase(citor);
			}
		}
	}

	inline virtual void push(Object* o) { children.push_back(o); }

	inline virtual void expire() {
		expired = true;
		for (auto itor = children.begin(); itor != children.end(); ++itor) {
			( *itor )->expire();
			delete* itor;
		}
		children.clear();
	}

	inline virtual Object* collide(Object* other, enum Category cat) {
		if (( this->cat == cat || cat == OBJ_ANY ) && ( other->cb.check(&( this->cb )) ))
			return this;

		for (auto itor = children.begin(); itor != children.end(); ++itor) {
			Object* collidee = ( *itor )->collide(other, cat);
			if (collidee != nullptr)
				return collidee;
		}
		return nullptr;
	}

	inline virtual Object& locate(vmath::vec3 pos) {
		this->pos = pos;
		return *this;
	}

	inline virtual Object& move(vmath::vec3 dpos) {
		pos += dpos;
		return *this;
	}

	inline virtual Object& exert(vmath::vec3 dvel) {
		vel += dvel;
		return *this;
	}

	inline virtual Object& setX(float x) { pos[0] = x; return *this; }
	inline virtual Object& setY(float y) { pos[1] = y; return *this; }
	inline virtual Object& setZ(float z) { pos[2] = z; return *this; }

	inline virtual Object& setXvel(float xspd) { vel[0] = xspd; return *this; }
	inline virtual Object& setYvel(float yspd) { vel[1] = yspd; return *this; }
	inline virtual Object& setZvel(float zspd) { vel[2] = zspd; return *this; }

	inline virtual Object& setXacc(float xacc) { acc[0] += xacc; return *this; }
	inline virtual Object& setYacc(float yacc) { acc[1] += yacc; return *this; }
	inline virtual Object& setZacc(float zacc) { acc[2] += zacc; return *this; }

	inline virtual vmath::vec3 getPos() { return pos; }
	inline virtual vmath::vec3 getVel() { return vel; }
	inline virtual vmath::vec3 getAcc() { return acc; }
	inline virtual bool isExpired() { return expired; }
	inline virtual enum Category getCat() { return cat; }
};
