//
//  Vector.h
//
//
//  Created by Narendra Umate on 10/4/12.
//
//

#pragma once

#include <cfloat>
#include <cmath>
#include <cstring>
#include <iostream>

#include "Matrix.h"

class Mat4;

enum Plane {
	l,
	r,
	b,
	t,
	f,
	n
};

enum Corner {
	lbf,
	lbn,
	ltf,
	ltn,
	rbf,
	rbn,
	rtf,
	rtn,
};

//  Vec1

class Vec1 {
public:
	union {
		struct {
			float x;
		};
		float data[1]; // pad
	};

	Vec1(void);
	Vec1(const float& x);
	Vec1(const float f[1]);

	void set(const float& x);
	Vec1 abs(void) const;
	float sqr(void) const;
	float norm(void) const;
	Vec1 normal(void) const;
	Vec1 reciprocal(void) const;
	void normalize(void);
	float product(void);

	float operator[](const int index) const;
	float& operator[](const int index);
	Vec1 operator-() const;
	Vec1 operator*(const Vec1& a) const;
	Vec1 operator*(const float& a) const;
	Vec1 operator/(const float& a) const;
	Vec1 operator+(const Vec1& a) const;
	Vec1 operator-(const Vec1& a) const;
	void operator+=(const Vec1& a);
	void operator-=(const Vec1& a);
	void operator/=(const Vec1& a);
	void operator/=(const float& a);
	void operator*=(const float& a);
	bool operator==(const Vec1& a) const;
	bool operator!=(const Vec1& a) const;
	Vec1 operator*(const Mat4& m) const;

	operator float() {
		return x;
	}

	friend std::ostream& operator<<(std::ostream& out, const Vec1& v);

	static const Vec1 zero;
	static const Vec1 one;
	static const Vec1 min;
	static const Vec1 max;
	static const Vec1 xxx;
};

//  Vec2

class Vec2 {
public:
	union {
		struct {
			float x; // s, u;
			float y; // t, v;
		};
		float data[2]; // pad
	};

	Vec2(void);
	Vec2(const float& x, const float& y);
	Vec2(const float f[2]);

	void set(const float& x, const float& y);
	Vec2 abs(void) const;
	float sqr(void) const;
	float norm(void) const;
	Vec2 normal(void) const;
	Vec2 reciprocal(void) const;
	void normalize(void);
	float product(void);

	float operator[](const int index) const;
	float& operator[](const int index);
	Vec2 operator-() const;
	Vec2 operator*(const Vec2& a) const;
	Vec2 operator*(const float& a) const;
	Vec2 operator/(const float& a) const;
	Vec2 operator+(const Vec2& a) const;
	Vec2 operator-(const Vec2& a) const;
	void operator+=(const Vec2& a);
	void operator-=(const Vec2& a);
	void operator/=(const Vec2& a);
	void operator/=(const float& a);
	void operator*=(const float& a);
	bool operator==(const Vec2& a) const;
	bool operator!=(const Vec2& a) const;
	Vec2 operator*(const Mat4& m) const;

	friend std::ostream& operator<<(std::ostream& out, const Vec2& v);

	static const Vec2 zero;
	static const Vec2 one;
	static const Vec2 min;
	static const Vec2 max;
	static const Vec2 xxx;
	static const Vec2 yyy;
};

//  Vec3

class Vec3 {
public:
	union {
		struct {
			float x; // u;
			float y; // v;
			float z; // w;
		};
		float data[3]; // pad
	};

	Vec3(void);
	Vec3(const float& x, const float& y, const float& z);
	Vec3(const float f[3]);

	void set(const float& x, const float& y, const float& z);
	Vec3 abs(void) const;
	float sqr(void) const;
	float norm(void) const;
	Vec3 normal(void) const;
	Vec3 reciprocal(void) const;
	void normalize(void);
	float product(void);

	float operator[](const int index) const;
	float& operator[](const int index);
	Vec3 operator-() const;
	Vec3 operator*(const Vec3& a) const;
	Vec3 operator*(const float& a) const;
	Vec3 operator/(const float& a) const;
	Vec3 operator+(const Vec3& a) const;
	Vec3 operator-(const Vec3& a) const;
	void operator+=(const Vec3& a);
	void operator-=(const Vec3& a);
	void operator/=(const Vec3& a);
	void operator/=(const float& a);
	void operator*=(const float& a);
	bool operator==(const Vec3& a) const;
	bool operator!=(const Vec3& a) const;
	Vec3 operator*(const Mat4& m) const;

	friend std::ostream& operator<<(std::ostream& out, const Vec3& v);

	static const Vec3 zero;
	static const Vec3 one;
	static const Vec3 min;
	static const Vec3 max;
	static const Vec3 xxx;
	static const Vec3 yyy;
	static const Vec3 zzz;
};

//  Vec4

class Vec4 {
public:
	union {
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float data[4]; // pad
	};

	Vec4(void);
	Vec4(const float& x, const float& y, const float& z, const float& w);
	Vec4(const float f[4]);
	Vec4(const Vec3& p);

	void set(const float& x, const float& y, const float& z, const float& w);
	Vec4 abs(void) const;
	float sqr(void) const;
	float norm(void) const;
	Vec4 normal(void) const;
	Vec4 reciprocal(void) const;
	void normalize(void);
	float product(void);
	Vec3 project(void) const;

	float operator[](const int index) const;
	float& operator[](const int index);
	Vec4 operator-() const;
	Vec4 operator*(const Vec4& a) const;
	Vec4 operator*(const float& a) const;
	Vec4 operator/(const float& a) const;
	Vec4 operator+(const Vec4& a) const;
	Vec4 operator-(const Vec4& a) const;
	void operator+=(const Vec4& a);
	void operator-=(const Vec4& a);
	void operator/=(const Vec4& a);
	void operator/=(const float& a);
	void operator*=(const float& a);
	bool operator==(const Vec4& a) const;
	bool operator!=(const Vec4& a) const;
	Vec4 operator*(const Mat4& m) const;

	friend std::ostream& operator<<(std::ostream& out, const Vec4& v);

	static const Vec4 zero;
	static const Vec4 one;
	static const Vec4 min;
	static const Vec4 max;
	static const Vec4 xxx;
	static const Vec4 yyy;
	static const Vec4 zzz;
	static const Vec4 www;
};

//  Functions

bool almostEqual(const Vec1& u, const Vec1& v, const float& e = M_EPSILON);
bool almostEqual(const Vec2& u, const Vec2& v, const float& e = M_EPSILON);
bool almostEqual(const Vec3& u, const Vec3& v, const float& e = M_EPSILON);
bool almostEqual(const Vec4& u, const Vec4& v, const float& e = M_EPSILON);

Vec4 directionVector(const Vec3& v);
Vec4 positionVector(const Vec3& v);

Vec1 minVec(const Vec1& u, const Vec1& v);
Vec2 minVec(const Vec2& u, const Vec2& v);
Vec3 minVec(const Vec3& u, const Vec3& v);
Vec4 minVec(const Vec4& u, const Vec4& v);

Vec1 maxVec(const Vec1& u, const Vec1& v);
Vec2 maxVec(const Vec2& u, const Vec2& v);
Vec3 maxVec(const Vec3& u, const Vec3& v);
Vec4 maxVec(const Vec4& u, const Vec4& v);

float dot(const Vec1& u, const Vec1& v);
float dot(const Vec2& u, const Vec2& v);
float dot(const Vec3& u, const Vec3& v);
float dot(const Vec4& u, const Vec4& v);

Vec3 cross(const Vec3& u, const Vec3& v);

float dist(const Vec1& u, const Vec1& v);
float dist(const Vec2& u, const Vec2& v);
float dist(const Vec3& u, const Vec3& v);
float dist(const Vec4& u, const Vec4& v);

float distSqr(const Vec1& u, const Vec1& v);
float distSqr(const Vec2& u, const Vec2& v);
float distSqr(const Vec3& u, const Vec3& v);
float distSqr(const Vec4& u, const Vec4& v);

Vec1 lerp(const Vec1& a, const Vec1& b, const float& t);
Vec2 lerp(const Vec2& a, const Vec2& b, const float& t);
Vec3 lerp(const Vec3& a, const Vec3& b, const float& t);
Vec4 lerp(const Vec4& a, const Vec4& b, const float& t);

Vec1 reflect(const Vec1& incident, const Vec1& normal);
Vec2 reflect(const Vec2& incident, const Vec2& normal);
Vec3 reflect(const Vec3& incident, const Vec3& normal);
Vec4 reflect(const Vec4& incident, const Vec4& normal);

Vec3 vectorTripleProduct(const Vec3& u, const Vec3& v, const Vec3& w);
float scalarTripleProduct(const Vec3& u, const Vec3& v, const Vec3& w);

Vec1 entrywiseProduct(const Vec1& u, const Vec1& v);
Vec2 entrywiseProduct(const Vec2& u, const Vec2& v);
Vec3 entrywiseProduct(const Vec3& u, const Vec3& v);
Vec4 entrywiseProduct(const Vec4& u, const Vec4& v);

Vec3 getDirection(const short& octant);

Vec4 planeFromPoints(const Vec3& a, const Vec3& b, const Vec3& c);
Vec3 pointFromPlanes(const Vec4& a, const Vec4& b, const Vec4& c);
