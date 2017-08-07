//
//  Math.cpp
//
//
//  Created by Narendra Umate on 4/13/13.
//
//

#include "Mathematics.h"

bool almostEqual(const float& a, const float& b, const float& e) {
	return fabs(a - b) < e;
}

float invsqrt(const float& value) {
	return 1.0f / sqrtf(value);
}

int clamp(const int& value, const int& min, const int& max) {
	// clang-format off
	return (value < min) ? min : (max < value) ? max : value;
	// clang-format on
}

float clamp(const float& value, const float& min, const float& max) {
	// clang-format off
	return (value < min) ? min : (max < value) ? max : value;
	// clang-format on
}

float degrees(const float& radians) {
	return radians * M_180_PI;
}

float radians(const float& degrees) {
	return degrees * M_PI_180;
}

float rand(const float& fmin, const float& fmax) {
	static bool init = false;
	if (!init) {
		init = true;
#if 1
		srand(0);
#else
		srand(time(NULL)); // time null will keep changing seed.
#endif
	}
	float f = (float)rand() / RAND_MAX;
	return fmin + f * (fmax - fmin);
}

float lerp(const float& a, const float& b, const float& t) {
	return (a - b) * t + b;
}

size_t hashCombine(const size_t& key, const size_t& value) {
	return key ^ (size_t(value) + 0x9e3779b9 + (key << 6) + (key >> 2));
}
