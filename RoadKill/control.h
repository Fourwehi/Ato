#pragma once

#include <vmath.h>
#include "key.h"

class Control {
private:
	inline static bool day = true;
	inline static bool ssao = true;

public:
	static bool isDay() { return day; }
	static bool isSSAO() { return ssao; }
	static vmath::vec4 getAmbient() { return day ? vmath::vec4(0.5, 0.75, 1.0, 0.5) : vmath::vec4(0.5, 1.0, 1.0, 0.2); }
	static vmath::vec4 getFog() { return day ? vmath::vec4(0.8, 0.9, 1.0, 1.0) : vmath::vec4(0.0, 0.1, 0.2, 1.0); }

	static void update() {
		if (Key::keyCheckPressed('2'))
			day = !day;

		if (Key::keyCheckPressed('3'))
			ssao = !ssao;
	}
};
