//
// BetterCarryingEachOther 2016.05.16
// light emitter class (header-only with inline)
//

#pragma once

#include <cstdlib>
#include <vmath.h>
#include "object.h"
#include "shader.h"
#include "resource.h"
#include "control.h"

class Light : public Object {
private:
	vmath::vec4 color;

public:
	inline Light(vmath::vec3 pos, vmath::vec4 color) : color(color) {
		locate(pos);
	}

	inline void update() {
		if (!Control::isDay()) {
			Shader::lightPush(vmath::vec4(pos, 1.0f), color); // Assuming vmath::vec4 can be constructed from vmath::vec3 and a float
		}
	}
};

class StreetLight : public Object {
private:
	float angle;
	vmath::vec3 dpos;

public:
	inline StreetLight(vmath::vec3 pos) {
		switch (rand() % 4) {
		case 0:
			angle = 180.0;
			dpos = vmath::vec3(16.0, 0.0, 96.0);
			break;
		case 1:
			angle = 270.0;
			dpos = vmath::vec3(0.0, 16.0, 96.0);
			break;
		case 2:
			angle = 0.0;
			dpos = vmath::vec3(-16.0, 0.0, 96.0);
			break;
		case 3:
			angle = 90.0;
			dpos = vmath::vec3(0.0, -16.0, 96.0);
			break;
		}
		locate(pos);
		push(new Light(pos + dpos, vmath::vec4(1.0, 0.8, 0.6, 100.0)));
	}

	inline void draw() const {
		Shader::push();
		Shader::translate(pos);
		Shader::rotateZ(angle);
		Shader::apply();
		Resource::Tex::streetlight.bind();
		Resource::Norm::flat.bind();
		Resource::streetlight.draw();
		Object::draw();
		Shader::pop();
	}
};
