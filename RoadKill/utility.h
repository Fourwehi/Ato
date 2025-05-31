//
// BetterCarryingEachOther 2016.03.15
// utility functions (header-only)
//

#pragma once

#include <vector>
#include <string>
#include <cstdlib> // For rand(), RAND_MAX
#include <algorithm> // For std::max, std::min (if clamping were used in flerp, not currently)
#include <vmath.h> // For vmath::vec3

// Note: 'using namespace std;' is avoided in this header. std:: is used explicitly.

inline float frand() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

inline float frandRange(float a, float b) {
    return frand() * (b - a) + a;
}

// Basic linear interpolation.
// For safe interpolation where x should be between 0 and 1, consider clamping x:
// float t = std::max(0.0f, std::min(1.0f, x)); return a * (1.0f - t) + b * t;
// Original RoadKill code did not clamp, so this version also does not.
inline float flerp(float a, float b, float x) {
    return a * (1.0f - x) + b * x;
}

inline vmath::vec3 lerpv3(vmath::vec3 a, vmath::vec3 b, float x) {
    return vmath::vec3(
        flerp(a[0], b[0], x),
        flerp(a[1], b[1], x),
        flerp(a[2], b[2], x)
    );
}

inline std::vector<std::string> tokenize(std::string str, const std::string& delim) { // Made delim const&
    std::vector<std::string> v;
    size_t seek = 0;

    while (seek < str.length()) { // Changed condition for clarity and safety
        size_t pos = str.find_first_of(delim, seek);

        if (pos == std::string::npos) { // Found no more delimiters
            v.push_back(str.substr(seek));
            break; // Exit loop
        }

        v.push_back(str.substr(seek, pos - seek));
        seek = pos + 1;
    }
    return v;
}
