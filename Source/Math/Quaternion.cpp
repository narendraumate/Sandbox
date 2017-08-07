//
//  Quaternion.cpp
//
//
//  Created by Narendra Umate on 3/8/13.
//
//

#include "Quaternion.h"

const Quat Quat::identity = Quat(0.0f, 0.0f, 0.0f, 1.0f);

Quat::Quat(void)
: x(0.0f)
, y(0.0f)
, z(0.0f)
, w(1.0f) {
}

Quat::Quat(const Vec4& vec4)
: x(vec4.x)
, y(vec4.y)
, z(vec4.z)
, w(vec4.w) {
}

Quat::Quat(const float& x, const float& y, const float& z, const float& w)
: x(x)
, y(y)
, z(z)
, w(w) {
}

Quat::Quat(const float& real, const Vec3& imag)
: x(imag.x)
, y(imag.y)
, z(imag.z)
, w(real) {
}

Quat::Quat(const Vec3& axis, const float& angle) {
	float angle_divide_2 = angle * 0.5f;
	Vec3 v = axis.normal() * sinf(angle_divide_2);
	x = v.x;
	y = v.y;
	z = v.z;
	w = /* s = */ cosf(angle_divide_2);
}

Quat::Quat(const Mat3& matrix) {
	float fourXSquaredMinus1 = matrix.d[0][0] - matrix.d[1][1] - matrix.d[2][2];
	float fourYSquaredMinus1 = matrix.d[1][1] - matrix.d[0][0] - matrix.d[2][2];
	float fourZSquaredMinus1 = matrix.d[2][2] - matrix.d[0][0] - matrix.d[1][1];
	float fourWSquaredMinus1 = matrix.d[0][0] + matrix.d[1][1] + matrix.d[2][2];
	int biggestIndex = 0;
	float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
	if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
	}
	if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}
	if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}
	float biggestVal = sqrtf(fourBiggestSquaredMinus1 + static_cast<float>(1)) * static_cast<float>(0.5);
	float mult = static_cast<float>(0.25) / biggestVal;
	switch (biggestIndex) {
		case 0: {
			w = biggestVal;
			x = (matrix.d[1][2] - matrix.d[2][1]) * mult;
			y = (matrix.d[2][0] - matrix.d[0][2]) * mult;
			z = (matrix.d[0][1] - matrix.d[1][0]) * mult;
		} break;
		case 1: {
			w = (matrix.d[1][2] - matrix.d[2][1]) * mult;
			x = biggestVal;
			y = (matrix.d[0][1] + matrix.d[1][0]) * mult;
			z = (matrix.d[2][0] + matrix.d[0][2]) * mult;
		} break;
		case 2: {
			w = (matrix.d[2][0] - matrix.d[0][2]) * mult;
			x = (matrix.d[0][1] + matrix.d[1][0]) * mult;
			y = biggestVal;
			z = (matrix.d[1][2] + matrix.d[2][1]) * mult;
		} break;
		case 3: {
			w = (matrix.d[0][1] - matrix.d[1][0]) * mult;
			x = (matrix.d[2][0] + matrix.d[0][2]) * mult;
			y = (matrix.d[1][2] + matrix.d[2][1]) * mult;
			z = biggestVal;
		} break;
		default: {
			// assert(false);
		} break;
	}
	normalize();
}

Quat::Quat(const Mat4& matrix) {
	float fourXSquaredMinus1 = matrix.d[0][0] - matrix.d[1][1] - matrix.d[2][2];
	float fourYSquaredMinus1 = matrix.d[1][1] - matrix.d[0][0] - matrix.d[2][2];
	float fourZSquaredMinus1 = matrix.d[2][2] - matrix.d[0][0] - matrix.d[1][1];
	float fourWSquaredMinus1 = matrix.d[0][0] + matrix.d[1][1] + matrix.d[2][2];
	int biggestIndex = 0;
	float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
	if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
	}
	if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}
	if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}
	float biggestVal = sqrtf(fourBiggestSquaredMinus1 + static_cast<float>(1)) * static_cast<float>(0.5);
	float mult = static_cast<float>(0.25) / biggestVal;
	switch (biggestIndex) {
		case 0: {
			w = biggestVal;
			x = (matrix.d[1][2] - matrix.d[2][1]) * mult;
			y = (matrix.d[2][0] - matrix.d[0][2]) * mult;
			z = (matrix.d[0][1] - matrix.d[1][0]) * mult;
		} break;
		case 1: {
			w = (matrix.d[1][2] - matrix.d[2][1]) * mult;
			x = biggestVal;
			y = (matrix.d[0][1] + matrix.d[1][0]) * mult;
			z = (matrix.d[2][0] + matrix.d[0][2]) * mult;
		} break;
		case 2: {
			w = (matrix.d[2][0] - matrix.d[0][2]) * mult;
			x = (matrix.d[0][1] + matrix.d[1][0]) * mult;
			y = biggestVal;
			z = (matrix.d[1][2] + matrix.d[2][1]) * mult;
		} break;
		case 3: {
			w = (matrix.d[0][1] - matrix.d[1][0]) * mult;
			x = (matrix.d[2][0] + matrix.d[0][2]) * mult;
			y = (matrix.d[1][2] + matrix.d[2][1]) * mult;
			z = biggestVal;
		} break;
		default: {
			// assert(false);
		} break;
	}
	normalize();
}

Quat::Quat(const Vec3& euler, const AxisOrderConvention& order) {
	// Reference https://github.com/mrdoob/three.js/blob/dev/src/math/Quaternion.js
	const float cx = cosf(euler.x / 2.0f);
	const float cy = cosf(euler.y / 2.0f);
	const float cz = cosf(euler.z / 2.0f);
	const float sx = sinf(euler.x / 2.0f);
	const float sy = sinf(euler.y / 2.0f);
	const float sz = sinf(euler.z / 2.0f);
	switch (order) {
		case X3Y2Z1: {
			x = sx * cy * cz + cx * sy * sz;
			y = cx * sy * cz - sx * cy * sz;
			z = cx * cy * sz + sx * sy * cz;
			w = cx * cy * cz - sx * sy * sz;
		} break;
		case Y3Z2X1: {
			x = sx * cy * cz + cx * sy * sz;
			y = cx * sy * cz + sx * cy * sz;
			z = cx * cy * sz - sx * sy * cz;
			w = cx * cy * cz - sx * sy * sz;
		} break;
		case Z3X2Y1: {
			x = sx * cy * cz - cx * sy * sz;
			y = cx * sy * cz + sx * cy * sz;
			z = cx * cy * sz + sx * sy * cz;
			w = cx * cy * cz - sx * sy * sz;
		} break;
		case X3Z2Y1: {
			x = sx * cy * cz - cx * sy * sz;
			y = cx * sy * cz - sx * cy * sz;
			z = cx * cy * sz + sx * sy * cz;
			w = cx * cy * cz + sx * sy * sz;
		} break;
		case Y3X2Z1: {
			x = sx * cy * cz + cx * sy * sz;
			y = cx * sy * cz - sx * cy * sz;
			z = cx * cy * sz - sx * sy * cz;
			w = cx * cy * cz + sx * sy * sz;
		} break;
		case Z3Y2X1: {
			x = sx * cy * cz - cx * sy * sz;
			y = cx * sy * cz + sx * cy * sz;
			z = cx * cy * sz - sx * sy * cz;
			w = cx * cy * cz + sx * sy * sz;
		} break;
	}
}

Vec3 Quat::axis() const {
	float axisLen = sqrtf(1.0f - w * w);
	if (axisLen < 1e-5) {
		// At lengths this small, direction is arbitrary
		return Vec3(1.0f, 0.0f, 0.0f);
	} else {
		return Vec3(x / axisLen, y / axisLen, z / axisLen);
	}
}

float Quat::angle() const {
	// w = cos(angle / 2)
	// angle = acos(w) * 2
	return 2.0f * acosf(w);
}

Quat Quat::inverse() const {
	float invmagsqr = 1.0f / dot(*this, *this);
	return Quat(-x * invmagsqr, -y * invmagsqr, -z * invmagsqr, w * invmagsqr);
}

Quat Quat::conjugate() const {
	return Quat(-x, -y, -z, w);
}

float Quat::norm(void) const {
	return sqrtf(x * x + y * y + z * z + w * w);
}

void Quat::normalize(void) {
	float mag = norm();
	x /= mag;
	y /= mag;
	z /= mag;
	w /= mag;
}

Mat3 Quat::mat3() const {
	std::cout << "called mat3" << std::endl;
	return Mat3(1 - 2 * (y * y + z * z), 2 * (x * y + z * w), 2 * (x * z - y * w),
				2 * (x * y - z * w), 1 - 2 * (z * z + x * x), 2 * (y * z + x * w),
				2 * (z * x + y * w), 2 * (y * z - x * w), 1 - 2 * (y * y + x * x));
}

Mat4 Quat::mat4() const {
	std::cout << "called mat4" << std::endl;
	return Mat4(1 - 2 * (y * y + z * z), 2 * (x * y + z * w), 2 * (x * z - y * w), 0.0f,
				2 * (x * y - z * w), 1 - 2 * (z * z + x * x), 2 * (y * z + x * w), 0.0f,
				2 * (z * x + y * w), 2 * (y * z - x * w), 1 - 2 * (y * y + x * x), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
}

Vec3 Quat::euler(const AxisOrderConvention& order) const {
	// Reference https://github.com/mrdoob/three.js/blob/dev/src/math/Quaternion.js
	Mat3 matrix = Mat3(1 - 2 * (y * y + z * z), 2 * (x * y + z * w), 2 * (x * z - y * w),
					   2 * (x * y - z * w), 1 - 2 * (z * z + x * x), 2 * (y * z + x * w),
					   2 * (z * x + y * w), 2 * (y * z - x * w), 1 - 2 * (y * y + x * x));

	float singularity = 0.999999f;
	Vec3 euler;
	switch (order) {
		case X3Y2Z1: {
			euler.y = asinf(clamp(matrix.d[2][0], -1.0f, 1.0f));
			if (singularity > std::abs(matrix.d[2][0])) {
				euler.x = atan2f(-matrix.d[2][1], matrix.d[2][2]);
				euler.z = atan2f(-matrix.d[1][0], matrix.d[0][0]);
			} else {
				euler.x = atan2f(matrix.d[1][2], matrix.d[1][1]);
				euler.z = 0.0f;
			}
		} break;

		case Y3Z2X1: {
			euler.z = asinf(clamp(matrix.d[0][1], -1.0f, 1.0f));
			if (singularity > std::abs(matrix.d[0][1])) {
				euler.x = atan2f(-matrix.d[2][1], matrix.d[1][1]);
				euler.y = atan2f(-matrix.d[0][2], matrix.d[0][0]);
			} else {
				euler.x = 0.0f;
				euler.y = atan2f(matrix.d[2][0], matrix.d[2][2]);
			}
		} break;

		case Z3X2Y1: {
			euler.x = asinf(clamp(matrix.d[1][2], -1.0f, 1.0f));
			if (singularity > std::abs(matrix.d[1][2])) {
				euler.y = atan2f(-matrix.d[0][2], matrix.d[2][2]);
				euler.z = atan2f(-matrix.d[1][0], matrix.d[1][1]);
			} else {
				euler.y = 0.0f;
				euler.z = atan2f(matrix.d[0][1], matrix.d[0][0]);
			}
		} break;

		case X3Z2Y1: {
			euler.z = asinf(-clamp(matrix.d[1][0], -1.0f, 1.0f));
			if (singularity > std::abs(matrix.d[1][0])) {
				euler.x = atan2f(matrix.d[1][2], matrix.d[1][1]);
				euler.y = atan2f(matrix.d[2][0], matrix.d[0][0]);
			} else {
				euler.x = atan2f(-matrix.d[2][1], matrix.d[2][2]);
				euler.y = 0.0f;
			}
		} break;

		case Y3X2Z1: {
			euler.x = asinf(clamp(-matrix.d[2][1], -1.0f, 1.0f));
			if (singularity > std::abs(matrix.d[2][1])) {
				euler.y = atan2f(matrix.d[2][0], matrix.d[2][2]);
				euler.z = atan2f(matrix.d[0][1], matrix.d[1][1]);
			} else {
				euler.y = atan2f(-matrix.d[0][2], matrix.d[0][0]);
				euler.z = 0.0f;
			}
		} break;

		case Z3Y2X1: {
			euler.y = asinf(-clamp(matrix.d[0][2], -1.0f, 1.0f));
			if (singularity > std::abs(matrix.d[0][2])) {
				euler.x = atan2f(matrix.d[1][2], matrix.d[2][2]);
				euler.z = atan2f(matrix.d[0][1], matrix.d[0][0]);
			} else {
				euler.x = 0.0f;
				euler.z = atan2f(-matrix.d[1][0], matrix.d[1][1]);
			}
		} break;
	}
	return euler;
}

Vec3 Quat::operator*(const Vec3& a) const {
	Quat aq(a.x, a.y, a.z, 0.0f);
	Quat at;
	Quat ar;
	Quat in = inverse();
	at = *this * aq;
	ar = at * in;
	return Vec3(ar.x, ar.y, ar.z);
}

Quat Quat::operator*(const Quat& a) const {
	// P * Q = (p0 + p1i + p2j + p3k)(q0 + q1i + q2j + q3k)
	// P * Q = (p0q0 − p1q1 − p2q2 − p3q3) + . . . i + . . . j + . . . k
	// P * Q = (p0 + p)(q0 + q)
	// P * Q = (p0q0 + p0q + q0p + pq)	//	pq = p x q - p . q	//	cross - dot
	// P * Q = (p0q0 - p.q) + (p0q + q0p + p x q)

	return Quat(a.w * x + a.x * w - a.y * z + a.z * y,
				a.w * y + a.x * z + a.y * w - a.z * x,
				a.w * z - a.x * y + a.y * x + a.z * w,
				a.w * w - a.x * x - a.y * y - a.z * z);
}

Quat Quat::operator/(const Quat& a) const {
	float invmagsqr = 1.0f / dot(*this, *this);
	return Quat((a.w * x - a.x * w - a.y * z + a.z * y) * invmagsqr,
				(a.w * y + a.x * z - a.y * w - a.z * x) * invmagsqr,
				(a.w * z - a.x * y + a.y * x - a.z * w) * invmagsqr,
				(a.w * w + a.x * x + a.y * y + a.z * z) * invmagsqr);
}

void Quat::operator*=(const Quat& a) {
	*this = *this * a;
}

void Quat::operator/=(const Quat& a) {
	*this = *this / a;
}

std::ostream& operator<<(std::ostream& out, const Quat& q) {
	out << q.x << ' ' << q.y << ' ' << q.z << ' ' << q.w;
	return out;
}

// Functions

bool almostEqual(const Quat& u, const Quat& v, const float& e) {
	return almostEqual(u.x, v.x, e) && almostEqual(u.y, v.y, e) && almostEqual(u.z, v.z, e) && almostEqual(u.w, v.w, e);
}

Quat lookat(const Vec3& direction) {
	Vec3 back = -direction.normal();
	Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 right = cross(up, back).normal();
	up = cross(back, right).normal();
	return rightupback(right, up, back);
}

Quat rotator(const Vec3& v0, const Vec3& v1) {
	// TODO Verify this
	// Reference Game Programming Gems 1
	// 2.10 The Shortest Arc Quaternion Page 217 Author Stan Melax
	Vec3 v0normal = v0.normal();
	Vec3 v1normal = v1.normal();
	float s_multiply_2 = sqrtf(2.0f * (1.0f + dot(v0normal, v1normal)));
	Vec3 v = cross(v0normal, v1normal) / s_multiply_2;
	return Quat(s_multiply_2 * 0.5f, v);
}

float dot(const Quat& q0, const Quat& q1) {
	return (q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w);
}

Quat lerp(const Quat& q0, const Quat& q1, const float& t) {
	return Quat(lerp(q0.x, q1.x, t), lerp(q0.y, q1.y, t),
				lerp(q0.z, q1.z, t), lerp(q0.w, q1.w, t));
}

Quat slerp(const Quat& q0, const Quat& q1, const float& t) {
	// TODO Implement this
	return Quat::identity;
}

Quat rightup(const Vec3& r, const Vec3& u) {
	Vec3 b = cross(r, u).normal();
	Mat3 matrix(r.x, r.y, r.z, u.x, u.y, u.z, b.x, b.y, b.z);
	return Quat(matrix);
}

Quat upback(const Vec3& u, const Vec3& b) {
	Vec3 r = cross(u, b).normal();
	Mat3 matrix(r.x, r.y, r.z, u.x, u.y, u.z, b.x, b.y, b.z);
	return Quat(matrix);
}

Quat backright(const Vec3& b, const Vec3& r) {
	Vec3 u = cross(b, r).normal();
	Mat3 matrix(r.x, r.y, r.z, u.x, u.y, u.z, b.x, b.y, b.z);
	return Quat(matrix);
}

Quat rightupback(const Vec3& r, const Vec3& u, const Vec3& b) {
	Mat3 matrix(r.x, r.y, r.z, u.x, u.y, u.z, b.x, b.y, b.z);
	return Quat(matrix);
}
