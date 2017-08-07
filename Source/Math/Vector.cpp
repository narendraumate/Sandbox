//
//  Vector.cpp
//
//
//  Created by Narendra Umate on 10/4/12.
//
//

#include "Vector.h"

const Vec1 Vec1::zero = Vec1(0.0f);
const Vec1 Vec1::one = Vec1(1.0f);
const Vec1 Vec1::min = Vec1(FLT_MIN);
const Vec1 Vec1::max = Vec1(FLT_MAX);
const Vec1 Vec1::xxx = Vec1(1.0f);

const Vec2 Vec2::zero = Vec2(0.0f, 0.0f);
const Vec2 Vec2::one = Vec2(1.0f, 1.0f);
const Vec2 Vec2::min = Vec2(FLT_MIN, FLT_MIN);
const Vec2 Vec2::max = Vec2(FLT_MAX, FLT_MAX);
const Vec2 Vec2::xxx = Vec2(1.0f, 0.0f);
const Vec2 Vec2::yyy = Vec2(0.0f, 1.0f);

const Vec3 Vec3::zero = Vec3(0.0f, 0.0f, 0.0f);
const Vec3 Vec3::one = Vec3(1.0f, 1.0f, 1.0f);
const Vec3 Vec3::min = Vec3(FLT_MIN, FLT_MIN, FLT_MIN);
const Vec3 Vec3::max = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
const Vec3 Vec3::xxx = Vec3(1.0f, 0.0f, 0.0f);
const Vec3 Vec3::yyy = Vec3(0.0f, 1.0f, 0.0f);
const Vec3 Vec3::zzz = Vec3(0.0f, 0.0f, 1.0f);

const Vec4 Vec4::zero = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
const Vec4 Vec4::one = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
const Vec4 Vec4::min = Vec4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN);
const Vec4 Vec4::max = Vec4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
const Vec4 Vec4::xxx = Vec4(1.0f, 0.0f, 0.0f, 0.0f);
const Vec4 Vec4::yyy = Vec4(0.0f, 1.0f, 0.0f, 0.0f);
const Vec4 Vec4::zzz = Vec4(0.0f, 0.0f, 1.0f, 0.0f);
const Vec4 Vec4::www = Vec4(0.0f, 0.0f, 0.0f, 1.0f);

//  Vec1

Vec1::Vec1(void)
: x(0.0f) {
}

Vec1::Vec1(const float& x)
: x(x) {
}

Vec1::Vec1(const float f[1])
: x(f[0]) {
}

void Vec1::set(const float& x) {
	this->x = x;
}

Vec1 Vec1::abs(void) const {
	return maxVec(*this, -*this);
}

float Vec1::sqr(void) const {
	return dot(*this, *this);
}

float Vec1::norm(void) const {
	return fabsf(x); // return x > 0.0f ? x : -x;//return sqrtf(dot(*this, *this));
}

Vec1 Vec1::normal(void) const {
	float magnitude = norm();
	return (*this / magnitude);
}

Vec1 Vec1::reciprocal(void) const {
	return Vec1(1.0f / x);
}

void Vec1::normalize(void) {
	float magnitude = norm();
	*this /= magnitude;
}

float Vec1::product(void) {
	return x;
}

float Vec1::operator[](const int index) const {
	return (&x)[index];
}

float& Vec1::operator[](const int index) {
	return (&x)[index];
}

Vec1 Vec1::operator-() const {
	return Vec1(-x);
}

Vec1 Vec1::operator*(const Vec1& a) const {
	return Vec1(x * a.x);
}

Vec1 Vec1::operator*(const float& a) const {
	return Vec1(x * a);
}

Vec1 Vec1::operator/(const float& a) const {
	return Vec1(x / a);
}

Vec1 Vec1::operator+(const Vec1& a) const {
	return Vec1(x + a.x);
}

Vec1 Vec1::operator-(const Vec1& a) const {
	return Vec1(x - a.x);
}

void Vec1::operator+=(const Vec1& a) {
	x += a.x;
}

void Vec1::operator-=(const Vec1& a) {
	x -= a.x;
}

void Vec1::operator/=(const Vec1& a) {
	x /= a.x;
}

void Vec1::operator/=(const float& a) {
	x /= a;
}

void Vec1::operator*=(const float& a) {
	x *= a;
}

bool Vec1::operator==(const Vec1& a) const {
	return (std::memcmp(this, &a, sizeof(Vec1)) == 0);
}

bool Vec1::operator!=(const Vec1& a) const {
	return (std::memcmp(this, &a, sizeof(Vec1)) != 0);
}

Vec1 Vec1::operator*(const Mat4& m) const {
	return Vec1(x * m.d[0][0] + m.d[0][3]);
}

std::ostream& operator<<(std::ostream& out, const Vec1& v) {
	out << v.x;
	return out;
}

//  Vec2

Vec2::Vec2(void)
: x(0.0f)
, y(0.0f) {
}

Vec2::Vec2(const float& x, const float& y)
: x(x)
, y(y) {
}

Vec2::Vec2(const float f[2])
: x(f[0])
, y(f[1]) {
}

void Vec2::set(const float& x, const float& y) {
	this->x = x;
	this->y = y;
}

Vec2 Vec2::abs(void) const {
	return maxVec(*this, -*this);
}

float Vec2::sqr(void) const {
	return dot(*this, *this);
}

float Vec2::norm(void) const {
	return sqrtf(dot(*this, *this));
}

Vec2 Vec2::normal(void) const {
	float magnitude = norm();
	return (*this / magnitude);
}

Vec2 Vec2::reciprocal(void) const {
	return Vec2(1.0f / x, 1.0f / y);
}

void Vec2::normalize(void) {
	float magnitude = norm();
	*this /= magnitude;
}

float Vec2::product(void) {
	return x * y;
}

float Vec2::operator[](const int index) const {
	return (&x)[index];
}

float& Vec2::operator[](const int index) {
	return (&x)[index];
}

Vec2 Vec2::operator-() const {
	return Vec2(-x, -y);
}

Vec2 Vec2::operator*(const Vec2& a) const {
	return Vec2(x * a.x, y * a.y);
}

Vec2 Vec2::operator*(const float& a) const {
	return Vec2(x * a, y * a);
}

Vec2 Vec2::operator/(const float& a) const {
	return Vec2(x / a, y / a);
}

Vec2 Vec2::operator+(const Vec2& a) const {
	return Vec2(x + a.x, y + a.y);
}

Vec2 Vec2::operator-(const Vec2& a) const {
	return Vec2(x - a.x, y - a.y);
}

void Vec2::operator+=(const Vec2& a) {
	x += a.x;
	y += a.y;
}

void Vec2::operator-=(const Vec2& a) {
	x -= a.x;
	y -= a.y;
}

void Vec2::operator/=(const Vec2& a) {
	x /= a.x;
	y /= a.y;
}

void Vec2::operator/=(const float& a) {
	x /= a;
	y /= a;
}

void Vec2::operator*=(const float& a) {
	x *= a;
	y *= a;
}

bool Vec2::operator==(const Vec2& a) const {
	return (std::memcmp(this, &a, sizeof(Vec2)) == 0);
}

bool Vec2::operator!=(const Vec2& a) const {
	return (std::memcmp(this, &a, sizeof(Vec1)) != 0);
}

Vec2 Vec2::operator*(const Mat4& m) const {
	return Vec2(x * m.d[0][0] + y * m.d[0][1] + m.d[0][3],
				x * m.d[1][0] + y * m.d[1][1] + m.d[1][3]);
}

std::ostream& operator<<(std::ostream& out, const Vec2& v) {
	out << v.x << ' ' << v.y;
	return out;
}

//  Vec3

Vec3::Vec3(void)
: x(0.0f)
, y(0.0f)
, z(0.0f) {
}

Vec3::Vec3(const float& x, const float& y, const float& z)
: x(x)
, y(y)
, z(z) {
}

Vec3::Vec3(const float f[3])
: x(f[0])
, y(f[1])
, z(f[2]) {
}

void Vec3::set(const float& x, const float& y, const float& z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3 Vec3::abs(void) const {
	return maxVec(*this, -*this);
}

float Vec3::sqr(void) const {
	return dot(*this, *this);
}

float Vec3::norm(void) const {
	return sqrtf(dot(*this, *this));
}

Vec3 Vec3::normal(void) const {
	float magnitude = norm();
	return (*this / magnitude);
}

Vec3 Vec3::reciprocal(void) const {
	return Vec3(1.0f / x, 1.0f / y, 1.0f / z);
}

void Vec3::normalize(void) {
	float magnitude = norm();
	*this /= magnitude;
}

float Vec3::product(void) {
	return x * y * z;
}

float Vec3::operator[](const int index) const {
	return (&x)[index];
}

float& Vec3::operator[](const int index) {
	return (&x)[index];
}

Vec3 Vec3::operator-() const {
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator*(const Vec3& a) const {
	return Vec3(x * a.x, y * a.y, z * a.z);
}

Vec3 Vec3::operator*(const float& a) const {
	return Vec3(x * a, y * a, z * a);
}

Vec3 Vec3::operator/(const float& a) const {
	return Vec3(x / a, y / a, z / a);
}

Vec3 Vec3::operator+(const Vec3& a) const {
	return Vec3(x + a.x, y + a.y, z + a.z);
}

Vec3 Vec3::operator-(const Vec3& a) const {
	return Vec3(x - a.x, y - a.y, z - a.z);
}

void Vec3::operator+=(const Vec3& a) {
	x += a.x;
	y += a.y;
	z += a.z;
}

void Vec3::operator-=(const Vec3& a) {
	x -= a.x;
	y -= a.y;
	z -= a.z;
}

void Vec3::operator/=(const Vec3& a) {
	x /= a.x;
	y /= a.y;
	z /= a.z;
}

void Vec3::operator/=(const float& a) {
	x /= a;
	y /= a;
	z /= a;
}

void Vec3::operator*=(const float& a) {
	x *= a;
	y *= a;
	z *= a;
}

bool Vec3::operator==(const Vec3& a) const {
	return (std::memcmp(this, &a, sizeof(Vec3)) == 0);
}

bool Vec3::operator!=(const Vec3& a) const {
	return (std::memcmp(this, &a, sizeof(Vec1)) != 0);
}

Vec3 Vec3::operator*(const Mat4& m) const {
	return Vec3(x * m.d[0][0] + y * m.d[0][1] + z * m.d[0][2] + m.d[0][3],
				x * m.d[1][0] + y * m.d[1][1] + z * m.d[1][2] + m.d[1][3],
				x * m.d[2][0] + y * m.d[2][1] + z * m.d[2][2] + m.d[2][3]);
}

std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	out << v.x << ' ' << v.y << ' ' << v.z;
	return out;
}

//  Vec4

Vec4::Vec4(void)
: x(0.0f)
, y(0.0f)
, z(0.0f)
, w(0.0f) {
}

Vec4::Vec4(const float& x, const float& y, const float& z, const float& w)
: x(x)
, y(y)
, z(z)
, w(w) {
}

Vec4::Vec4(const float f[4])
: x(f[0])
, y(f[1])
, z(f[2])
, w(f[3]) {
}

Vec4::Vec4(const Vec3& p)
: x(p.x)
, y(p.y)
, z(p.z)
, w(1.0) {
}

void Vec4::set(const float& x, const float& y, const float& z, const float& w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vec4 Vec4::abs(void) const {
	return maxVec(*this, -*this);
}

float Vec4::sqr(void) const {
	return dot(*this, *this);
}

float Vec4::norm(void) const {
	return sqrtf(dot(*this, *this));
}

Vec4 Vec4::normal(void) const {
	float magnitude = norm();
	return (*this / magnitude);
}

Vec4 Vec4::reciprocal(void) const {
	return Vec4(1.0f / x, 1.0f / y, 1.0f / z, 1.0f / w);
}

void Vec4::normalize(void) {
	float magnitude = norm();
	*this /= magnitude;
}

float Vec4::product(void) {
	return x * y * z * w;
}

Vec3 Vec4::project(void) const {
	return Vec3(x, y, z);
}

float Vec4::operator[](const int index) const {
	return (&x)[index];
}

float& Vec4::operator[](const int index) {
	return (&x)[index];
}

Vec4 Vec4::operator-() const {
	return Vec4(-x, -y, -z, -w);
}

Vec4 Vec4::operator*(const Vec4& a) const {
	return Vec4(x * a.x, y * a.y, z * a.z, w * a.w);
}

Vec4 Vec4::operator*(const float& a) const {
	return Vec4(x * a, y * a, z * a, w * a);
}

Vec4 Vec4::operator/(const float& a) const {
	return Vec4(x / a, y / a, z / a, w / a);
}

Vec4 Vec4::operator+(const Vec4& a) const {
	return Vec4(x + a.x, y + a.y, z + a.z, w + a.w);
}

Vec4 Vec4::operator-(const Vec4& a) const {
	return Vec4(x - a.x, y - a.y, z - a.z, w - a.w);
}

void Vec4::operator+=(const Vec4& a) {
	x += a.x;
	y += a.y;
	z += a.z;
	w += a.w;
}

void Vec4::operator-=(const Vec4& a) {
	x -= a.x;
	y -= a.y;
	z -= a.z;
	w -= a.w;
}

void Vec4::operator/=(const Vec4& a) {
	x /= a.x;
	y /= a.y;
	z /= a.z;
	w /= a.w;
}

void Vec4::operator/=(const float& a) {
	x /= a;
	y /= a;
	z /= a;
	w /= a;
}

void Vec4::operator*=(const float& a) {
	x *= a;
	y *= a;
	z *= a;
	w *= a;
}

bool Vec4::operator==(const Vec4& a) const {
	return (std::memcmp(this, &a, sizeof(Vec4)) == 0);
}

bool Vec4::operator!=(const Vec4& a) const {
	return (std::memcmp(this, &a, sizeof(Vec4)) != 0);
}

Vec4 Vec4::operator*(const Mat4& m) const {
	return Vec4(x * m.d[0][0] + y * m.d[0][1] + z * m.d[0][2] + w * m.d[0][3],
				x * m.d[1][0] + y * m.d[1][1] + z * m.d[1][2] + w * m.d[1][3],
				x * m.d[2][0] + y * m.d[2][1] + z * m.d[2][2] + w * m.d[2][3],
				x * m.d[3][0] + y * m.d[3][1] + z * m.d[3][2] + w * m.d[3][3]);
}

std::ostream& operator<<(std::ostream& out, const Vec4& v) {
	out << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
	return out;
}

//  Functions

bool almostEqual(const Vec1& u, const Vec1& v, const float& e) {
	return almostEqual(u.x, v.x, e);
}

bool almostEqual(const Vec2& u, const Vec2& v, const float& e) {
	return almostEqual(u.x, v.x, e) && almostEqual(u.y, v.y, e);
}

bool almostEqual(const Vec3& u, const Vec3& v, const float& e) {
	return almostEqual(u.x, v.x, e) && almostEqual(u.y, v.y, e) && almostEqual(u.z, v.z, e);
}

bool almostEqual(const Vec4& u, const Vec4& v, const float& e) {
	return almostEqual(u.x, v.x, e) && almostEqual(u.y, v.y, e) && almostEqual(u.z, v.z, e) && almostEqual(u.w, v.w, e);
}

Vec4 directionVector(const Vec3& v) {
	return Vec4(v.x, v.y, v.z, 0.0f); // This has been verified correct.
}

Vec4 positionVector(const Vec3& v) {
	return Vec4(v.x, v.y, v.z, 1.0f); // This has been verified correct.
}

Vec1 minVec(const Vec1& u, const Vec1& v) {
	return Vec1(u.x < v.x ? u.x : v.x);
}

Vec2 minVec(const Vec2& u, const Vec2& v) {
	return Vec2(minVec(u.x, v.x), minVec(u.y, v.y));
}

Vec3 minVec(const Vec3& u, const Vec3& v) {
	return Vec3(minVec(u.x, v.x), minVec(u.y, v.y), minVec(u.z, v.z));
}

Vec4 minVec(const Vec4& u, const Vec4& v) {
	return Vec4(minVec(u.x, v.x), minVec(u.y, v.y), minVec(u.z, v.z), minVec(u.w, v.w));
}

Vec1 maxVec(const Vec1& u, const Vec1& v) {
	return Vec1(u.x > v.x ? u.x : v.x);
}

Vec2 maxVec(const Vec2& u, const Vec2& v) {
	return Vec2(maxVec(u.x, v.x), maxVec(u.y, v.y));
}

Vec3 maxVec(const Vec3& u, const Vec3& v) {
	return Vec3(maxVec(u.x, v.x), maxVec(u.y, v.y), maxVec(u.z, v.z));
}

Vec4 maxVec(const Vec4& u, const Vec4& v) {
	return Vec4(maxVec(u.x, v.x), maxVec(u.y, v.y), maxVec(u.z, v.z), maxVec(u.w, v.w));
}

float dot(const Vec1& u, const Vec1& v) {
	return u.x * v.x;
}

float dot(const Vec2& u, const Vec2& v) {
	return u.x * v.x + u.y * v.y;
}

float dot(const Vec3& u, const Vec3& v) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

float dot(const Vec4& u, const Vec4& v) {
	return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

Vec3 cross(const Vec3& u, const Vec3& v) {
	return Vec3(u.y * v.z - v.y * u.z, v.x * u.z - u.x * v.z, u.x * v.y - v.x * u.y);
}

float dist(const Vec1& u, const Vec1& v) {
	return fabsf(u.x - v.x);
}

float dist(const Vec2& u, const Vec2& v) {
	return sqrtf((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y));
}

float dist(const Vec3& u, const Vec3& v) {
	return sqrtf((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y) + (u.z - v.z) * (u.z - v.z));
}

float dist(const Vec4& u, const Vec4& v) {
	return sqrtf((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y) + (u.z - v.z) * (u.z - v.z) + (u.w - v.w) * (u.w - v.w));
}

float distSqr(const Vec1& u, const Vec1& v) {
	return (u.x - v.x) * (u.x - v.x);
}

float distSqr(const Vec2 u, const Vec2& v) {
	return (u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y);
}

float distSqr(const Vec3& u, const Vec3& v) {
	return ((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y) + (u.z - v.z) * (u.z - v.z));
}

float distSqr(const Vec4& u, const Vec4& v) {
	return ((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y) + (u.z - v.z) * (u.z - v.z) + (u.w - v.w) * (u.w - v.w));
}

Vec1 lerp(const Vec1& a, const Vec1& b, const float& t) {
	return (a - b) * t + b;
}

Vec2 lerp(const Vec2& a, const Vec2& b, const float& t) {
	return (a - b) * t + b;
}

Vec3 lerp(const Vec3& a, const Vec3& b, const float& t) {
	return (a - b) * t + b;
}

Vec4 lerp(const Vec4& a, const Vec4& b, const float& t) {
	return (a - b) * t + b;
}

Vec1 reflect(const Vec1& incident, const Vec1& normal) {
	return incident - normal * 2.0f * dot(incident, normal);
}

Vec2 reflect(const Vec2& incident, const Vec2& normal) {
	return incident - normal * 2.0f * dot(incident, normal);
}

Vec3 reflect(const Vec3& incident, const Vec3& normal) {
	return incident - normal * 2.0f * dot(incident, normal);
}

Vec4 reflect(const Vec4& incident, const Vec4& normal) {
	return incident - normal * 2.0f * dot(incident, normal);
}

Vec3 vectorTripleProduct(const Vec3& u, const Vec3& v, const Vec3& w) {
	// a x (b x c) = (a . c) b - (a . b) c
	return cross(u, cross(v, w));
}

float scalarTripleProduct(const Vec3& u, const Vec3& v, const Vec3& w) {
	// a . (b x c) = b . (c x a) = c . (a x b)
	return dot(u, cross(v, w));
}

Vec1 entrywiseProduct(const Vec1& u, const Vec1& v) {
	return Vec1(u.x * v.x);
}

Vec2 entrywiseProduct(const Vec2& u, const Vec2& v) {
	return Vec2(u.x * v.x, u.y * v.y);
}

Vec3 entrywiseProduct(const Vec3& u, const Vec3& v) {
	return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

Vec4 entrywiseProduct(const Vec4& u, const Vec4& v) {
	return Vec4(u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w);
}

Vec3 getDirection(const short& octant) {
	switch (octant % 8) {
		case 0:
			return Vec3(-1, -1, -1);
		case 1:
			return Vec3(-1, -1, +1);
		case 2:
			return Vec3(-1, +1, -1);
		case 3:
			return Vec3(-1, +1, +1);
		case 4:
			return Vec3(+1, -1, -1);
		case 5:
			return Vec3(+1, -1, +1);
		case 6:
			return Vec3(+1, +1, -1);
		case 7:
			return Vec3(+1, +1, +1);
		default:
			return Vec3(0, 0, 0);
	}
}

Vec4 planeFromPoints(const Vec3& a, const Vec3& b, const Vec3& c) {
	const Vec3 n = cross(b - a, c - a).normal();
	return Vec4(n.x, n.y, n.z, -dot(a, n));
}

Vec3 pointFromPlanes(const Vec4& a, const Vec4& b, const Vec4& c) {
	const float den = scalarTripleProduct(a.project(), b.project(), c.project());
	if (almostEqual(den, 0.0f)) {
		return Vec3::zero;
	} else {
		Vec3 res = cross(b.project(), c.project()) * a.w + cross(c.project(), a.project()) * b.w + cross(a.project(), b.project()) * c.w;
		return res * (-1.0f / den);
	}
}
