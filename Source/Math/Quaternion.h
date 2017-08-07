//
//  Quaternion.h
//
//
//  Created by Narendra Umate on 3/8/13.
//
//

#pragma once

#include "Convention.h"
#include "Mathematics.h"
#include "Matrix.h"
#include "Vector.h"

class Mat3;
class Mat4;
class Vec3;
class Vec4;

class Quat {

public:
	// s + v
	// w + xi + yj + zk
	// s = w = cos(theta / 2)
	// v = (x, y, z) = sin(theta / 2) * rotation axis vector

	float x, y, z, w; // x, y, z, w instead of w, x, y, z for Vec4 match

	Quat(void);
	Quat(const Vec4& vec4);
	Quat(const float& x, const float& y, const float& z, const float& w);
	Quat(const float& s, const Vec3& vec3);
	Quat(const Vec3& axis, const float& radangle);
	Quat(const Mat3& matrix);
	Quat(const Mat4& matrix);
	Quat(const Vec3& right, const Vec3& up, const Vec3& back);
	Quat(const Vec3& euler, const AxisOrderConvention& order = Y3X2Z1);

	Vec3 axis() const;
	float angle() const;
	Quat inverse() const;
	Quat conjugate() const;
	float norm(void) const;
	Quat normal(void) const;
	Quat normalize(void);
	Mat3 mat3() const;
	Mat4 mat4() const;
	Vec3 euler(const AxisOrderConvention& order = Y3X2Z1) const;

	Vec3 operator*(const Vec3& a) const;
	Quat operator+(const Quat& a) const;
	Quat operator-(const Quat& a) const;
	Quat operator*(const Quat& a) const; // q0 * q1 != q1 * q0 noncommutative
	Quat operator/(const Quat& a) const; // q0 / q1 is possible
	void operator*=(const Quat& a);
	void operator/=(const Quat& a);

	static const Quat identity;
};

// Functions

bool almostEqual(const Quat& u, const Quat& v);

float dot(const Quat& q0, const Quat& q1);
Quat lookat(const Vec3& direction);
Quat rotator(const Vec3& v0, const Vec3& v1); // q that rotates v0 to v1
Quat lerp(const Quat& q0, const Quat& q1, const float& t);
Quat slerp(const Quat& q0, const Quat& q1, const float& t);
