//
//  Color.cpp
//
//
//  Created by Narendra Umate on 10/5/12.
//
//

#include "Color.h"

//  Color3f

Color3f::Color3f(const Color3i& color)
#if defined __LITTLE_ENDIAN__ //  R8G8B8 Reverse
: r((float)((color >> 0) & 0x000000FF) / 255.0f)
, g((float)((color >> 8) & 0x000000FF) / 255.0f)
, b((float)((color >> 16) & 0x000000FF) / 255.0f)
#else //  B8G8R8 Straight
: b((float)((color >> 0) & 0x000000FF) / 255.0f)
, g((float)((color >> 8) & 0x000000FF) / 255.0f)
, r((float)((color >> 16) & 0x000000FF) / 255.0f)
#endif
{
}

Color3f::Color3f(const Color3f& color) {
	std::memcpy(this, &color, sizeof(Color3f));
}

Color3f::Color3f(Color3f&& color) {
	std::memcpy(this, &color, sizeof(Color3f));
	std::memset(&color, 0, sizeof(Color3f));
}

Color3f::Color3f(const unsigned char& r, const unsigned char& g, const unsigned char& b)
: r((float)r / 255.0f)
, g((float)g / 255.0f)
, b((float)b / 255.0f) {
}

Color3f::Color3f(const float& r, const float& g, const float& b)
: r(r)
, g(g)
, b(b) {
}

Color3f::Color3f(const Vec3& v)
: r(v.x)
, g(v.y)
, b(v.z) {
}

Color3f& Color3f::operator=(const Color3f& u) {
	if (this != &u) {
		std::memcpy(this, &u, sizeof(Color3f));
	}
	return *this;
}

Color3f& Color3f::operator=(Color3f&& u) {
	if (this != &u) {
		std::memcpy(this, &u, sizeof(Color3f));
		std::memset(&u, 0, sizeof(Color3f));
	}
	return *this;
}

Color3f Color3f::operator+(const Color3f& u) const {
	return Color3f(r + u.r,
				   g + u.g,
				   b + u.b);
}

Color3f Color3f::operator-(const Color3f& u) const {
	return Color3f(r - u.r,
				   g - u.g,
				   b - u.b);
}

Color3f Color3f::operator*(const Color3f& u) const {
	return Color3f(r * u.r,
				   g * u.g,
				   b * u.b);
}

Color3f Color3f::operator/(const Color3f& u) const {
	return Color3f(r / u.r,
				   g / u.g,
				   b / u.b);
}

Color3f Color3f::operator*(const float& u) const {
	return Color3f(r * u,
				   g * u,
				   b * u);
}

Color3f Color3f::operator/(const float& u) const {
	return Color3f(r / u,
				   g / u,
				   b / u);
}

Color3f Color3f::operator+() const {
	return *this;
}

Color3f Color3f::operator-() const {
	return Color3f(-r, -g, -b);
}

Color3f& Color3f::operator+=(const Color3f& u) {
	*this = *this + u;
	return *this;
}

Color3f& Color3f::operator-=(const Color3f& u) {
	*this = *this - u;
	return *this;
}

Color3f& Color3f::operator/=(const Color3f& u) {
	*this = *this / u;
	return *this;
}

Color3f& Color3f::operator*=(const Color3f& u) {
	*this = *this * u;
	return *this;
}

Color3f& Color3f::operator/=(const float u) {
	*this = *this / u;
	return *this;
}

Color3f& Color3f::operator*=(const float u) {
	*this = *this * u;
	return *this;
}

bool Color3f::operator==(const Color3f& u) const {
	return (std::memcmp(this, &u, sizeof(Color3f)) == 0);
}

bool Color3f::operator!=(const Color3f& u) const {
	return (std::memcmp(this, &u, sizeof(Color3f)) != 0);
}

Color3f Color3f::random() {
	return Color3f(rand(0.0f, 1.0f), rand(0.0f, 1.0f), rand(0.0f, 1.0f));
}

Vec3 Color3f::vec3() const {
	return data;
}

Color3i Color3f::color() const {
	return (Color3i)RGB((uint8_t)(r * 255.0f),
						(uint8_t)(g * 255.0f),
						(uint8_t)(b * 255.0f));
}

std::ostream& operator<<(std::ostream& out, const Color3f& u) {
	out << u.r << ' ' << u.g << ' ' << u.b;
	return out;
}

//    Functions

Color3i color(const Vec3& v) {
	return (Color3i)RGB((uint8_t)(v.x * 255.0f),
						(uint8_t)(v.y * 255.0f),
						(uint8_t)(v.z * 255.0f));
}

Color3f lerp(const Color3f& u, const Color3f& v, const float& t) {
	return (u - v) * t + v;
}

//  Color4f

Color4f::Color4f(const Color4i& color)
#if defined __LITTLE_ENDIAN__ //  R8G8B8A8 Reverse
: r((float)((color >> 0) & 0x000000FF) / 255.0f)
, g((float)((color >> 8) & 0x000000FF) / 255.0f)
, b((float)((color >> 16) & 0x000000FF) / 255.0f)
, a((float)((color >> 24) & 0x000000FF) / 255.0f)
#else //  A8B8G8R8 Straight
: a((float)((color >> 0) & 0x000000FF) / 255.0f)
, b((float)((color >> 8) & 0x000000FF) / 255.0f)
, g((float)((color >> 16) & 0x000000FF) / 255.0f)
, r((float)((color >> 24) & 0x000000FF) / 255.0f)
#endif
{
}

Color4f::Color4f(const Color4f& color) {
	std::memcpy(this, &color, sizeof(Color4f));
}

Color4f::Color4f(Color4f&& color) {
	std::memcpy(this, &color, sizeof(Color4f));
	std::memset(&color, 0, sizeof(Color4f));
}

Color4f::Color4f(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
: r((float)r / 255.0f)
, g((float)g / 255.0f)
, b((float)b / 255.0f)
, a((float)a / 255.0f) {
}

Color4f::Color4f(const float& r, const float& g, const float& b, const float& a)
: r(r)
, g(g)
, b(b)
, a(a) {
}

Color4f::Color4f(const Vec3& v, const float& a)
: r(v.x)
, g(v.y)
, b(v.z)
, a(a) {
}

Color4f::Color4f(const Vec4& v)
: r(v.x)
, g(v.y)
, b(v.z)
, a(v.w) {
}

Color4f& Color4f::operator=(const Color4f& u) {
	if (this != &u) {
		std::memcpy(this, &u, sizeof(Color4f));
	}
	return *this;
}

Color4f& Color4f::operator=(Color4f&& u) {
	if (this != &u) {
		std::memcpy(this, &u, sizeof(Color4f));
		std::memset(&u, 0, sizeof(Color4f));
	}
	return *this;
}

Color4f Color4f::operator+(const Color4f& u) const {
	return Color4f(r + u.r,
				   g + u.g,
				   b + u.b,
				   a + u.a);
}

Color4f Color4f::operator-(const Color4f& u) const {
	return Color4f(r - u.r,
				   g - u.g,
				   b - u.b,
				   a - u.a);
}

Color4f Color4f::operator*(const Color4f& u) const {
	return Color4f(r * u.r,
				   g * u.g,
				   b * u.b,
				   a * u.a);
}

Color4f Color4f::operator/(const Color4f& u) const {
	return Color4f(r / u.r,
				   g / u.g,
				   b / u.b,
				   a / u.a);
}

Color4f Color4f::operator*(const float& u) const {
	return Color4f(r * u,
				   g * u,
				   b * u,
				   a * u);
}

Color4f Color4f::operator/(const float& u) const {
	return Color4f(r / u,
				   g / u,
				   b / u,
				   a / u);
}

Color4f Color4f::operator+() const {
	return *this;
}

Color4f Color4f::operator-() const {
	return Color4f(-r, -g, -b, -a);
}

Color4f& Color4f::operator+=(const Color4f& u) {
	*this = *this + u;
	return *this;
}

Color4f& Color4f::operator-=(const Color4f& u) {
	*this = *this - u;
	return *this;
}

Color4f& Color4f::operator/=(const Color4f& u) {
	*this = *this / u;
	return *this;
}

Color4f& Color4f::operator*=(const Color4f& u) {
	*this = *this * u;
	return *this;
}

Color4f& Color4f::operator/=(const float u) {
	*this = *this / u;
	return *this;
}

Color4f& Color4f::operator*=(const float u) {
	*this = *this * u;
	return *this;
}

bool Color4f::operator==(const Color4f& u) const {
	return (std::memcmp(this, &u, sizeof(Color4f)) == 0);
}

bool Color4f::operator!=(const Color4f& u) const {
	return (std::memcmp(this, &u, sizeof(Color4f)) != 0);
}

Color4f Color4f::random() {
	return Color4f(rand(0.0f, 1.0f), rand(0.0f, 1.0f), rand(0.0f, 1.0f), 1.0f);
}

Vec4 Color4f::vec4() const {
	return data;
}

Color4i Color4f::color() const {
	return (Color4i)RGBA((uint8_t)(r * 255.0f),
						 (uint8_t)(g * 255.0f),
						 (uint8_t)(b * 255.0f),
						 (uint8_t)(a * 255.0f));
}

std::ostream& operator<<(std::ostream& out, const Color4f& u) {
	out << u.r << ' ' << u.g << ' ' << u.b << ' ' << u.a;
	return out;
}

//	Functions

Color4i color(const Vec3& v, const float& a) {
	return (Color4i)RGBA((uint8_t)(v.x * 255.0f),
						 (uint8_t)(v.y * 255.0f),
						 (uint8_t)(v.z * 255.0f),
						 (uint8_t)(a * 255.0f));
}

Color4i color(const Vec4& v) {
	return (Color4i)RGBA((uint8_t)(v.x * 255.0f),
						 (uint8_t)(v.y * 255.0f),
						 (uint8_t)(v.z * 255.0f),
						 (uint8_t)(v.w * 255.0f));
}

Color4f lerp(const Color4f& u, const Color4f& v, const float& t) {
	return (u - v) * t + v;
}
