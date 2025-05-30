//
// BetterCarryingEachOther 2016.03.15
// utility functions
//

#pragma once

#include <vector>
#include <vmath.h> // Added vmath.h

using namespace std;

float frand();
float frandRange(float a, float b);
float flerp(float a, float b, float x);
vmath::vec3 lerpv3(vmath::vec3 a, vmath::vec3 b, float x); // Changed vec3 to vmath::vec3
vector<string> tokenize(string str, string delim);
// float degToRad(float x); // Removed, use vmath::radians instead
// float radToDeg(float x); // Removed, use vmath::degrees instead