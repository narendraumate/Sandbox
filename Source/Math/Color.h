//
//  Color.h
//
//
//  Created by Narendra Umate on 10/5/12.
//
//

#pragma once

#include <cstdint>
#include <cstring>
#include <iostream>

#include "Mathematics.h"
#include "Vector.h"

/*----------------------------------------------------------------------------*/
/*  Endian  First Byte Low Address  Last Byte High Address  1000	Decimal   */
/*  Big		Most Significant		Least Significant		03E8	Straight  */
/*  Little  Least Significant		Most Significant		E803	Reverse   */
/*----------------------------------------------------------------------------*/

#if defined __LITTLE_ENDIAN__
#define RGBA(r, g, b, a) (a << 24 | b << 16 | g << 8 | r) //  A8B8G8R8
#else
#define RGBA(r, g, b, a) (r << 24 | g << 16 | b << 8 | a) //  R8G8B8A8
#endif

#if defined __LITTLE_ENDIAN__
#define RGB(r, g, b) (b << 16 | g << 8 | r) //  B8G8R8
#else
#define RGB(r, g, b) (r << 16 | g << 8 | b) //  R8G8B8
#endif

// Color3i

enum Color3i {
	Color3iRed = RGB(0xFF, 0x00, 0x00),
	Color3iYellow = RGB(0xFF, 0xFF, 0x00),
	Color3iMagenta = RGB(0xFF, 0x00, 0xFF),
	Color3iGreen = RGB(0x00, 0xFF, 0x00),
	Color3iCyan = RGB(0x00, 0xFF, 0xFF),
	Color3iBlue = RGB(0x00, 0x00, 0xFF),
	Color3iWhite = RGB(0xFF, 0xFF, 0xFF),
	Color3iGray = RGB(0x80, 0x80, 0x80),
	Color3iBlack = RGB(0x00, 0x00, 0x00),
};

// Color3f

class Color3f {

public:
	Color3f(const Color3i& color = Color3iBlack);
	Color3f(const Color3f& color);
	Color3f(Color3f&& color);
	Color3f(const unsigned char& r, const unsigned char& g, const unsigned char& b);
	Color3f(const float& r, const float& g, const float& b);
	Color3f(const Vec3& v);

	Color3f& operator=(const Color3f& u);
	Color3f& operator=(Color3f&& u);
	Color3f operator+(const Color3f& u) const;
	Color3f operator-(const Color3f& u) const;
	Color3f operator*(const Color3f& u) const;
	Color3f operator/(const Color3f& u) const;

	Color3f operator*(const float& u) const;
	Color3f operator/(const float& u) const;
	Color3f operator+() const;
	Color3f operator-() const;

	Color3f& operator+=(const Color3f& u);
	Color3f& operator-=(const Color3f& u);
	Color3f& operator/=(const Color3f& u);
	Color3f& operator*=(const Color3f& u);
	Color3f& operator/=(const float u);
	Color3f& operator*=(const float u);

	bool operator==(const Color3f& u) const;
	bool operator!=(const Color3f& u) const;

	static Color3f random();
	Vec3 vec3() const;
	Color3i color() const;

	friend std::ostream& operator<<(std::ostream& out, const Color3f& u);

	union {
		struct {
			float r;
			float g;
			float b;
		};
		float data[3];
	};
};

// Color4i

enum Color4i {
	Color4iRed = RGBA(0xFF, 0x00, 0x00, 0xFF),
	Color4iYellow = RGBA(0xFF, 0xFF, 0x00, 0xFF),
	Color4iMagenta = RGBA(0xFF, 0x00, 0xFF, 0xFF),
	Color4iGreen = RGBA(0x00, 0xFF, 0x00, 0xFF),
	Color4iCyan = RGBA(0x00, 0xFF, 0xFF, 0xFF),
	Color4iBlue = RGBA(0x00, 0x00, 0xFF, 0xFF),
	Color4iWhite = RGBA(0xFF, 0xFF, 0xFF, 0xFF),
	Color4iGray = RGBA(0x80, 0x80, 0x80, 0xFF),
	Color4iBlack = RGBA(0x00, 0x00, 0x00, 0xFF),
};

// Color4f

class Color4f {

public:
	Color4f(const Color4i& color = Color4iBlack);
	Color4f(const Color4f& color);
	Color4f(Color4f&& color);
	Color4f(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a = 255);
	Color4f(const float& r, const float& g, const float& b, const float& a = 1.0f);
	Color4f(const Vec3& v, const float& a = 1.0f);
	Color4f(const Vec4& v);

	Color4f& operator=(const Color4f& u);
	Color4f& operator=(Color4f&& u);
	Color4f operator+(const Color4f& u) const;
	Color4f operator-(const Color4f& u) const;
	Color4f operator*(const Color4f& u) const;
	Color4f operator/(const Color4f& u) const;

	Color4f operator*(const float& u) const;
	Color4f operator/(const float& u) const;
	Color4f operator+() const;
	Color4f operator-() const;

	Color4f& operator+=(const Color4f& u);
	Color4f& operator-=(const Color4f& u);
	Color4f& operator/=(const Color4f& u);
	Color4f& operator*=(const Color4f& u);
	Color4f& operator/=(const float u);
	Color4f& operator*=(const float u);

	bool operator==(const Color4f& u) const;
	bool operator!=(const Color4f& u) const;

	static Color4f random();
	Vec4 vec4() const;
	Color4i color() const;

	friend std::ostream& operator<<(std::ostream& out, const Color4f& u);

	union {
		struct {
			float r;
			float g;
			float b;
			float a;
		};
		float data[4];
	};
};

//  Functions

Color4i color(const Vec3& v, const float& a = 1.0f);
Color4i color(const Vec4& v);
Color4f lerp(const Color4f& u, const Color4f& v, const float& t);
