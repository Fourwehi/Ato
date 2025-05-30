#pragma once

#include <vmath.h>
// #include "key.h" // Removed

class Control {
// private: // Members made public static
public:
	inline static bool day = true;
	inline static bool ssao = true;

public:
	static bool isDay() { return day; }
	static bool isSSAO() { return ssao; }
	static vmath::vec4 getAmbient() { return day ? vmath::vec4(0.5f, 0.75f, 1.0f, 0.5f) : vmath::vec4(0.5f, 1.0f, 1.0f, 0.2f); } // Added 'f' suffix for float literals
	static vmath::vec4 getFog() { return day ? vmath::vec4(0.8f, 0.9f, 1.0f, 1.0f) : vmath::vec4(0.0f, 0.1f, 0.2f, 1.0f); }     // Added 'f' suffix for float literals

	// static void update() { // Removed
	//	if (Key::keyCheckPressed('2'))
	//		day = !day;
	//
	//	if (Key::keyCheckPressed('3'))
	//		ssao = !ssao;
	// }
};
