//
//  Matrix.h
//
//
//  Created by Narendra Umate on 11/4/12.
//
//

#pragma once

#include <cstring>

#include "Mathematics.h"
#include "Quaternion.h"
#include "Vector.h"

#if defined(__APPLE__) || defined(__linux__) || defined(_WIN32)
//	OpenGL
#define COLUMN_MAJOR
#endif // defined(__APPLE__) || defined(__linux__) || defined(_WIN32)

class Vec2;
class Vec3;
class Vec4;
class Mat3;
class Mat3;
class Mat4;
class Quat;

//  Mat2

class Mat2 {
public:
	union {
		float d[2][2];
		float r[4];
	};

	Mat2(void);
	Mat2(const float& v00, const float& v01,
		 const float& v10, const float& v11);
	Mat2(const Mat3& m);

	Mat2 transpose() const;
	Mat2 inverse() const;
	float determinant() const;

	Vec2 operator*(const Vec2& v) const;
	Mat2 operator*(const Mat2& v) const;
	Mat2 operator*(const float& f) const;
	Mat2 operator/(const float& f) const;
	bool operator==(const Mat2& m) const;
	bool operator!=(const Mat2& m) const;

	friend std::ostream& operator<<(std::ostream& out, const Mat2& m);

	static const Mat2 identity;
	static const Mat2 zero;
};

//  Mat3

class Mat3 {
public:
	union {
		float d[3][3];
		float r[9];
	};

	Mat3(void);
	Mat3(const float& v00, const float& v01, const float& v02,
		 const float& v10, const float& v11, const float& v12,
		 const float& v20, const float& v21, const float& v22);
	Mat3(const Mat4& m);

	static Mat3 translate(const Vec2& v);
	static Mat3 rotate(const float& theta);
	static Mat3 scale(const Vec2& s);

	Mat3 transpose() const;
	Mat3 inverse() const;
	Mat3 noscaleinverse() const;
	float determinant() const;

	Vec3 operator*(const Vec3& v) const;
	Mat3 operator*(const Mat3& v) const;
	Mat3 operator*(const float& f) const;
	Mat3 operator/(const float& f) const;
	bool operator==(const Mat3& m) const;
	bool operator!=(const Mat3& m) const;

	friend std::ostream& operator<<(std::ostream& out, const Mat3& m);

	static const Mat3 identity;
	static const Mat3 zero;
};

//  Mat4

class Mat4 {
public:
	union {
		float d[4][4];
		float r[16];
	};

	Mat4(void);
	Mat4(const float& v00, const float& v01, const float& v02, const float& v03,
		 const float& v10, const float& v11, const float& v12, const float& v13,
		 const float& v20, const float& v21, const float& v22, const float& v23,
		 const float& v30, const float& v31, const float& v32, const float& v33);

	static Mat4 translate(const Vec3& v);
	static Mat4 rotate(const Quat& q);
	static Mat4 scale(const Vec3& s);

	Mat4 transpose() const;
	Mat4 inverse() const;
	Mat4 noscaleinverse() const;
	float determinant() const;

	Vec4 operator*(const Vec4& v) const;
	Mat4 operator*(const Mat4& m) const;
	Mat4 operator*(const float& f) const;
	Mat4 operator/(const float& f) const;
	bool operator==(const Mat4& m) const;
	bool operator!=(const Mat4& m) const;

	friend std::ostream& operator<<(std::ostream& out, const Mat4& m);

	static const Mat4 identity;
	static const Mat4 zero;
};

// Functions

Mat4 convert(Mat4 m, AxisDirectionConvention dst, AxisDirectionConvention src);
