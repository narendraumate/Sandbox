//
//  Mathematics.h
//
//
//  Created by Narendra Umate on 4/13/13.
//
//

#pragma once

#define _USE_MATH_DEFINES

#include <cfloat>
#include <cmath>
#include <stdlib.h>
#include <time.h>

#if defined(_WIN32)
#include <math.h>
#endif

// double precision literals are either
// 1 place before point and 35 after or
// 0 place before point and 36 after it

#define M_PI_180 M_PI / 180.0f
#define M_180_PI 180.0f / M_PI
#define M_DEGREE_TO_RADIAN M_PI_180
#define M_RADIAN_TO_DEGREE M_180_PI

// precision is different for platforms
#if defined(__APPLE__)
#define M_EPSILON 0.000001f // tested
#elif defined(_WIN32)
#define M_EPSILON 0.000001f // tested
#else
#define M_EPSILON 0.000001f // tested
#endif

//	Windows UINT
#ifndef UINT
typedef unsigned int UINT;
#endif // UINT

//	MacOS uint
#ifndef uint
typedef unsigned int uint;
#endif // uint

bool almostEqual(const float& a, const float& b, const float& e = M_EPSILON);

int clamp(const int& value, const int& min, const int& max);
float clamp(const float& value, const float& min, const float& max);

float degrees(const float& radians);
float radians(const float& degrees);

float rand(const float& fmin, const float& fmax);
float lerp(const float& a, const float& b, const float& t);

size_t hashCombine(const size_t& a, const size_t& b);
