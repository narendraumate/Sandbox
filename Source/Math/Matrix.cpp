//
//  Matrix.cpp
//
//
//  Created by Narendra Umate on 11/4/12.
//
//

#include "Matrix.h"

//  Reference
//  http://www.geometrictools.com/Documentation/LaplaceExpansionTheorem.pdf
//  http://www.cs.rochester.edu/u/brown/Crypto/assts/projects/adj.html

//  Mat2

Mat2::Mat2(void) {
	d[0][0] = (1.0f);
	d[1][0] = (0.0f);
	d[0][1] = (0.0f);
	d[1][1] = (1.0f);
}

Mat2::Mat2(const float& v00, const float& v01,
		   const float& v10, const float& v11) {
	d[0][0] = (v00);
	d[1][0] = (v10);
	d[0][1] = (v01);
	d[1][1] = (v11);
}

Mat2::Mat2(const Mat3& m) {
	d[0][0] = (m.d[0][0]);
	d[1][0] = (m.d[1][0]);
	d[0][1] = (m.d[0][1]);
	d[1][1] = (m.d[1][1]);
}

Mat2 Mat2::transpose() const {
	return Mat2(d[0][0], d[1][0],
				d[0][1], d[1][1]);
}

Mat2 Mat2::inverse() const {
	return Mat2(+d[1][1], -d[1][0],
				-d[0][1], +d[0][0])
		/ determinant();
}

float Mat2::determinant() const {
	return d[0][0] * d[1][1] - d[0][1] * d[1][0];
}

Vec2 Mat2::operator*(const Vec2& v) const {
	return Vec2(d[0][0] * v.x + d[1][0] * v.y,
				d[0][1] * v.x + d[1][1] * v.y);
}

Mat2 Mat2::operator*(const Mat2& m) const {
	return Mat2(d[0][0] * m.d[0][0] + d[1][0] * m.d[0][1], d[0][1] * m.d[0][0] + d[1][1] * m.d[0][1],
				d[0][0] * m.d[1][0] + d[1][0] * m.d[1][1], d[0][1] * m.d[1][0] + d[1][1] * m.d[1][1]);
}

Mat2 Mat2::operator*(const float& f) const {
	Mat2 m = *this;
	m.d[0][0] *= f;
	m.d[1][0] *= f;
	m.d[0][1] *= f;
	m.d[1][1] *= f;
	return m;
}

Mat2 Mat2::operator/(const float& f) const {
	Mat2 m = *this;
	m.d[0][0] /= f;
	m.d[1][0] /= f;
	m.d[0][1] /= f;
	m.d[1][1] /= f;
	return m;
}

bool Mat2::operator==(const Mat2& m) const {
	return (std::memcmp(this, &m, sizeof(Mat2)) == 0);
}

bool Mat2::operator!=(const Mat2& m) const {
	return (std::memcmp(this, &m, sizeof(Mat2)) != 0);
}

std::ostream& operator<<(std::ostream& out, const Mat2& m) {
	out << m.r[0];
	for (int i = 1; i < 4; ++i) {
		out << " " << m.r[i];
	}
	return out;
}

const Mat2 Mat2::identity = Mat2(1.0f, 0.0f,
								 0.0f, 1.0f);

const Mat2 Mat2::zero = Mat2(0.0f, 0.0f,
							 0.0f, 0.0f);

//  Mat3

Mat3::Mat3(void) {
	d[0][0] = (1.0f);
	d[1][0] = (0.0f);
	d[2][0] = (0.0f);
	d[0][1] = (0.0f);
	d[1][1] = (1.0f);
	d[2][1] = (0.0f);
	d[0][2] = (0.0f);
	d[1][2] = (0.0f);
	d[2][2] = (1.0f);
}

Mat3::Mat3(const float& v00, const float& v01, const float& v02,
		   const float& v10, const float& v11, const float& v12,
		   const float& v20, const float& v21, const float& v22) {
	d[0][0] = (v00);
	d[1][0] = (v10);
	d[2][0] = (v20);
	d[0][1] = (v01);
	d[1][1] = (v11);
	d[2][1] = (v21);
	d[0][2] = (v02);
	d[1][2] = (v12);
	d[2][2] = (v22);
}

Mat3::Mat3(const Mat4& m) {
	d[0][0] = (m.d[0][0]);
	d[1][0] = (m.d[1][0]);
	d[2][0] = (m.d[2][0]);
	d[0][1] = (m.d[0][1]);
	d[1][1] = (m.d[1][1]);
	d[2][1] = (m.d[2][1]);
	d[0][2] = (m.d[0][2]);
	d[1][2] = (m.d[1][2]);
	d[2][2] = (m.d[2][2]);
}

Mat3 Mat3::translate(const Vec2& v) {
	return Mat3(1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				v.x, v.y, 1.0f);
}

Mat3 Mat3::rotate(const float& theta) {
	float cosT = cosf(theta); //  theta in radians
	float sinT = sinf(theta); //  theta in radians
	return Mat3(cosT, sinT, 0.0f,
				-sinT, cosT, 0.0f,
				0.0f, 0.0f, 1.0f);
}

Mat3 Mat3::scale(const Vec2& s) {
	return Mat3(s.x, 0.0f, 0.0f,
				0.0f, s.y, 0.0f,
				0.0f, 0.0f, 1.0f);
}

Mat3 Mat3::transpose() const {
	return Mat3(d[0][0], d[1][0], d[2][0],
				d[0][1], d[1][1], d[2][1],
				d[0][2], d[1][2], d[2][2]);
}

Mat3 Mat3::inverse() const {
	float det = determinant();
	if (det == 0.0f) {
		// Handle singular matrix
		// Return identity matrix or throw an error, depending on your desired behavior
		return Mat3::identity;
	}

	float invDet = 1.0f / det;
	Mat3 result;

	result.d[0][0] = (+d[1][1] * d[2][2] - d[1][2] * d[2][1]) * invDet;
	result.d[0][1] = (-d[0][1] * d[2][2] + d[0][2] * d[2][1]) * invDet;
	result.d[0][2] = (+d[0][1] * d[1][2] - d[0][2] * d[1][1]) * invDet;
	result.d[1][0] = (-d[1][0] * d[2][2] + d[1][2] * d[2][0]) * invDet;
	result.d[1][1] = (+d[0][0] * d[2][2] - d[0][2] * d[2][0]) * invDet;
	result.d[1][2] = (-d[0][0] * d[1][2] + d[0][2] * d[1][0]) * invDet;
	result.d[2][0] = (+d[1][0] * d[2][1] - d[1][1] * d[2][0]) * invDet;
	result.d[2][1] = (-d[0][0] * d[2][1] + d[0][1] * d[2][0]) * invDet;
	result.d[2][2] = (+d[0][0] * d[1][1] - d[0][1] * d[1][0]) * invDet;

	return result;
}

Mat3 Mat3::noscaleinverse() const {
	Mat2 oldr = Mat2(d[0][0], d[0][1], d[1][0], d[1][1]);
	Vec2 oldt = Vec2(d[0][2], d[1][2]);
	Mat2 newr = oldr.transpose();
	Vec2 newt = (newr * oldt) * -1.0f;

	return Mat3(newr.d[0][0], newr.d[0][1], 0.0f,
				newr.d[1][0], newr.d[1][1], 0.0f,
				newt.data[0], newt.data[1], 1.0f);
}

float Mat3::determinant() const {
	return d[0][0] * (d[1][1] * d[2][2] - d[1][2] * d[2][1]) - d[1][0] * (d[0][1] * d[2][2] - d[0][2] * d[2][1]) + d[2][0] * (d[0][1] * d[1][2] - d[0][2] * d[1][1]);
}

Vec3 Mat3::operator*(const Vec3& v) const {
	return Vec3(d[0][0] * v.x + d[1][0] * v.y + d[2][0] * v.z,
				d[0][1] * v.x + d[1][1] * v.y + d[2][1] * v.z,
				d[0][2] * v.x + d[1][2] * v.y + d[2][2] * v.z);
}

Mat3 Mat3::operator*(const Mat3& m) const {
	return Mat3(d[0][0] * m.d[0][0] + d[1][0] * m.d[0][1] + d[2][0] * m.d[0][2],
				d[0][1] * m.d[0][0] + d[1][1] * m.d[0][1] + d[2][1] * m.d[0][2],
				d[0][2] * m.d[0][0] + d[1][2] * m.d[0][1] + d[2][2] * m.d[0][2],
				d[0][0] * m.d[1][0] + d[1][0] * m.d[1][1] + d[2][0] * m.d[1][2],
				d[0][1] * m.d[1][0] + d[1][1] * m.d[1][1] + d[2][1] * m.d[1][2],
				d[0][2] * m.d[1][0] + d[1][2] * m.d[1][1] + d[2][2] * m.d[1][2],
				d[0][0] * m.d[2][0] + d[1][0] * m.d[2][1] + d[2][0] * m.d[2][2],
				d[0][1] * m.d[2][0] + d[1][1] * m.d[2][1] + d[2][1] * m.d[2][2],
				d[0][2] * m.d[2][0] + d[1][2] * m.d[2][1] + d[2][2] * m.d[2][2]);
}

Mat3 Mat3::operator*(const float& f) const {
	Mat3 m = *this;
	m.d[0][0] *= f;
	m.d[1][0] *= f;
	m.d[2][0] *= f;
	m.d[0][1] *= f;
	m.d[1][1] *= f;
	m.d[2][1] *= f;
	m.d[0][2] *= f;
	m.d[1][2] *= f;
	m.d[2][2] *= f;
	return m;
}

Mat3 Mat3::operator/(const float& f) const {
	Mat3 m = *this;
	m.d[0][0] /= f;
	m.d[1][0] /= f;
	m.d[2][0] /= f;
	m.d[0][1] /= f;
	m.d[1][1] /= f;
	m.d[2][1] /= f;
	m.d[0][2] /= f;
	m.d[1][2] /= f;
	m.d[2][2] /= f;
	return m;
}

bool Mat3::operator==(const Mat3& m) const {
	return (std::memcmp(this, &m, sizeof(Mat3)) == 0);
}

bool Mat3::operator!=(const Mat3& m) const {
	return (std::memcmp(this, &m, sizeof(Mat3)) != 0);
}

std::ostream& operator<<(std::ostream& out, const Mat3& m) {
	out << m.r[0];
	for (int i = 1; i < 9; ++i) {
		out << " " << m.r[i];
	}
	return out;
}

const Mat3 Mat3::identity = Mat3(1.0f, 0.0f, 0.0f,
								 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 1.0f);

const Mat3 Mat3::zero = Mat3(0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f);

//  Mat4

Mat4::Mat4(void) {
	d[0][0] = (1.0f);
	d[1][0] = (0.0f);
	d[2][0] = (0.0f);
	d[3][0] = (0.0f);
	d[0][1] = (0.0f);
	d[1][1] = (1.0f);
	d[2][1] = (0.0f);
	d[3][1] = (0.0f);
	d[0][2] = (0.0f);
	d[1][2] = (0.0f);
	d[2][2] = (1.0f);
	d[3][2] = (0.0f);
	d[0][3] = (0.0f);
	d[1][3] = (0.0f);
	d[2][3] = (0.0f);
	d[3][3] = (1.0f);
}

Mat4::Mat4(const float& v00, const float& v01, const float& v02, const float& v03,
		   const float& v10, const float& v11, const float& v12, const float& v13,
		   const float& v20, const float& v21, const float& v22, const float& v23,
		   const float& v30, const float& v31, const float& v32, const float& v33) {
	d[0][0] = (v00);
	d[1][0] = (v10);
	d[2][0] = (v20);
	d[3][0] = (v30);
	d[0][1] = (v01);
	d[1][1] = (v11);
	d[2][1] = (v21);
	d[3][1] = (v31);
	d[0][2] = (v02);
	d[1][2] = (v12);
	d[2][2] = (v22);
	d[3][2] = (v32);
	d[0][3] = (v03);
	d[1][3] = (v13);
	d[2][3] = (v23);
	d[3][3] = (v33);
}

Mat4 Mat4::translate(const Vec3& v) {
	return Mat4(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				v.x, v.y, v.z, 1.0f);
}

Mat4 Mat4::rotate(const Quat& q) {
	const float x = q.x;
	const float y = q.y;
	const float z = q.z;
	const float w = q.w;
	return Mat4(1 - 2 * (y * y + z * z), 2 * (x * y + z * w), 2 * (x * z - y * w), 0.0f,
				2 * (x * y - z * w), 1 - 2 * (z * z + x * x), 2 * (y * z + x * w), 0.0f,
				2 * (z * x + y * w), 2 * (y * z - x * w), 1 - 2 * (y * y + x * x), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
}

Mat4 Mat4::scale(const Vec3& s) {
	return Mat4(s.x, 0.0f, 0.0f, 0.0f,
				0.0f, s.y, 0.0f, 0.0f,
				0.0f, 0.0f, s.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
}

Mat4 Mat4::transpose() const {
	return Mat4(d[0][0], d[1][0], d[2][0], d[3][0],
				d[0][1], d[1][1], d[2][1], d[3][1],
				d[0][2], d[1][2], d[2][2], d[3][2],
				d[0][3], d[1][3], d[2][3], d[3][3]);
}

Mat4 Mat4::inverse() const {
	const float s0 = d[0][0] * d[1][1] - d[0][1] * d[1][0];
	const float s1 = d[0][0] * d[2][1] - d[0][1] * d[2][0];
	const float s2 = d[0][0] * d[3][1] - d[0][1] * d[3][0];
	const float s3 = d[1][0] * d[2][1] - d[1][1] * d[2][0];
	const float s4 = d[1][0] * d[3][1] - d[1][1] * d[3][0];
	const float s5 = d[2][0] * d[3][1] - d[2][1] * d[3][0];

	const float c0 = d[0][2] * d[1][3] - d[0][3] * d[1][2];
	const float c1 = d[0][2] * d[2][3] - d[0][3] * d[2][2];
	const float c2 = d[0][2] * d[3][3] - d[0][3] * d[3][2];
	const float c3 = d[1][2] * d[2][3] - d[1][3] * d[2][2];
	const float c4 = d[1][2] * d[3][3] - d[1][3] * d[3][2];
	const float c5 = d[2][2] * d[3][3] - d[2][3] * d[3][2];

	const float d0 = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;

	return Mat4(+d[1][1] * c5 - d[2][1] * c4 + d[3][1] * c3, -d[0][1] * c5 + d[2][1] * c2 - d[3][1] * c1, +d[0][1] * c4 - d[1][1] * c2 + d[3][1] * c0, -d[0][1] * c3 + d[1][1] * c1 - d[2][1] * c0,
				-d[1][0] * c5 + d[2][0] * c4 - d[3][0] * c3, +d[0][0] * c5 - d[2][0] * c2 + d[3][0] * c1, -d[0][0] * c4 + d[1][0] * c2 - d[3][0] * c0, +d[0][0] * c3 - d[1][0] * c1 + d[2][0] * c0,
				+d[1][3] * s5 - d[2][3] * s4 + d[3][3] * s3, -d[0][3] * s5 + d[2][3] * s2 - d[3][3] * s1, +d[0][3] * s4 - d[1][3] * s2 + d[3][3] * s0, -d[0][3] * s3 + d[1][3] * s1 - d[2][3] * s0,
				-d[1][2] * s5 + d[2][2] * s4 - d[3][2] * s3, +d[0][2] * s5 - d[2][2] * s2 + d[3][2] * s1, -d[0][2] * s4 + d[1][2] * s2 - d[3][2] * s0, +d[0][2] * s3 - d[1][2] * s1 + d[2][2] * s0)
		/ d0;
}

Mat4 Mat4::noscaleinverse() const {
	Mat3 oldr = Mat3(d[0][0], d[0][1], d[0][2], d[1][0], d[1][1], d[1][2], d[2][0], d[2][1], d[2][2]);
	Vec3 oldt = Vec3(d[3][0], d[3][1], d[3][2]);
	Mat3 newr = oldr.transpose();
	Vec3 newt = oldr.transpose() * oldt * -1.0f;
	return Mat4(newr.d[0][0], newr.d[0][1], newr.d[0][2], 0.0f,
				newr.d[1][0], newr.d[1][1], newr.d[1][2], 0.0f,
				newr.d[2][0], newr.d[2][1], newr.d[2][2], 0.0f,
				newt.data[0], newt.data[1], newt.data[2], 1.0f);
}

float Mat4::determinant() const {
#if 1
	const float s0 = d[0][0] * d[1][1] - d[0][1] * d[1][0];
	const float s1 = d[0][0] * d[2][1] - d[0][1] * d[2][0];
	const float s2 = d[0][0] * d[3][1] - d[0][1] * d[3][0];
	const float s3 = d[1][0] * d[2][1] - d[1][1] * d[2][0];
	const float s4 = d[1][0] * d[3][1] - d[1][1] * d[3][0];
	const float s5 = d[2][0] * d[3][1] - d[2][1] * d[3][0];

	const float c0 = d[0][2] * d[1][3] - d[0][3] * d[1][2];
	const float c1 = d[0][2] * d[2][3] - d[0][3] * d[2][2];
	const float c2 = d[0][2] * d[3][3] - d[0][3] * d[3][2];
	const float c3 = d[1][2] * d[2][3] - d[1][3] * d[2][2];
	const float c4 = d[1][2] * d[3][3] - d[1][3] * d[3][2];
	const float c5 = d[2][2] * d[3][3] - d[2][3] * d[3][2];

	const float d0 = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;

	return d0;
#else
	return (d[0][0] * (d[1][1] * (d[2][2] * d[3][3] - d[2][3] * d[3][2]) - d[2][1] * (d[1][2] * d[3][3] - d[1][3] * d[3][2]) + d[3][1] * (d[1][2] * d[2][3] - d[1][3] * d[2][2])) - d[1][0] * (d[0][1] * (d[2][2] * d[3][3] - d[2][3] * d[3][2]) - d[2][1] * (d[0][2] * d[3][3] - d[0][3] * d[3][2]) + d[3][1] * (d[0][2] * d[2][3] - d[0][3] * d[2][2])) + d[2][0] * (d[0][1] * (d[1][2] * d[3][3] - d[1][3] * d[3][2]) - d[1][1] * (d[0][2] * d[3][3] - d[0][3] * d[3][2]) + d[3][1] * (d[0][2] * d[1][3] - d[0][3] * d[1][2])) - d[3][0] * (d[0][1] * (d[1][2] * d[2][3] - d[1][3] * d[2][2]) - d[1][1] * (d[0][2] * d[2][3] - d[0][3] * d[2][2]) + d[2][1] * (d[0][2] * d[1][3] - d[0][3] * d[1][2])));
#endif
}

Vec4 Mat4::operator*(const Vec4& v) const {
	return Vec4(d[0][0] * v.x + d[1][0] * v.y + d[2][0] * v.z + d[3][0] * v.w,
				d[0][1] * v.x + d[1][1] * v.y + d[2][1] * v.z + d[3][1] * v.w,
				d[0][2] * v.x + d[1][2] * v.y + d[2][2] * v.z + d[3][2] * v.w,
				d[0][3] * v.x + d[1][3] * v.y + d[2][3] * v.z + d[3][3] * v.w);
}

Mat4 Mat4::operator*(const Mat4& m) const {
	return Mat4(d[0][0] * m.d[0][0] + d[1][0] * m.d[0][1] + d[2][0] * m.d[0][2] + d[3][0] * m.d[0][3],
				d[0][1] * m.d[0][0] + d[1][1] * m.d[0][1] + d[2][1] * m.d[0][2] + d[3][1] * m.d[0][3],
				d[0][2] * m.d[0][0] + d[1][2] * m.d[0][1] + d[2][2] * m.d[0][2] + d[3][2] * m.d[0][3],
				d[0][3] * m.d[0][0] + d[1][3] * m.d[0][1] + d[2][3] * m.d[0][2] + d[3][3] * m.d[0][3],
				d[0][0] * m.d[1][0] + d[1][0] * m.d[1][1] + d[2][0] * m.d[1][2] + d[3][0] * m.d[1][3],
				d[0][1] * m.d[1][0] + d[1][1] * m.d[1][1] + d[2][1] * m.d[1][2] + d[3][1] * m.d[1][3],
				d[0][2] * m.d[1][0] + d[1][2] * m.d[1][1] + d[2][2] * m.d[1][2] + d[3][2] * m.d[1][3],
				d[0][3] * m.d[1][0] + d[1][3] * m.d[1][1] + d[2][3] * m.d[1][2] + d[3][3] * m.d[1][3],
				d[0][0] * m.d[2][0] + d[1][0] * m.d[2][1] + d[2][0] * m.d[2][2] + d[3][0] * m.d[2][3],
				d[0][1] * m.d[2][0] + d[1][1] * m.d[2][1] + d[2][1] * m.d[2][2] + d[3][1] * m.d[2][3],
				d[0][2] * m.d[2][0] + d[1][2] * m.d[2][1] + d[2][2] * m.d[2][2] + d[3][2] * m.d[2][3],
				d[0][3] * m.d[2][0] + d[1][3] * m.d[2][1] + d[2][3] * m.d[2][2] + d[3][3] * m.d[2][3],
				d[0][0] * m.d[3][0] + d[1][0] * m.d[3][1] + d[2][0] * m.d[3][2] + d[3][0] * m.d[3][3],
				d[0][1] * m.d[3][0] + d[1][1] * m.d[3][1] + d[2][1] * m.d[3][2] + d[3][1] * m.d[3][3],
				d[0][2] * m.d[3][0] + d[1][2] * m.d[3][1] + d[2][2] * m.d[3][2] + d[3][2] * m.d[3][3],
				d[0][3] * m.d[3][0] + d[1][3] * m.d[3][1] + d[2][3] * m.d[3][2] + d[3][3] * m.d[3][3]);
}

Mat4 Mat4::operator*(const float& f) const {
	Mat4 m = *this;
	m.d[0][0] *= f;
	m.d[1][0] *= f;
	m.d[2][0] *= f;
	m.d[3][0] *= f;
	m.d[0][1] *= f;
	m.d[1][1] *= f;
	m.d[2][1] *= f;
	m.d[3][1] *= f;
	m.d[0][2] *= f;
	m.d[1][2] *= f;
	m.d[2][2] *= f;
	m.d[3][2] *= f;
	m.d[0][3] *= f;
	m.d[1][3] *= f;
	m.d[2][3] *= f;
	m.d[3][3] *= f;
	return m;
}

Mat4 Mat4::operator/(const float& f) const {
	Mat4 m = *this;
	m.d[0][0] /= f;
	m.d[1][0] /= f;
	m.d[2][0] /= f;
	m.d[3][0] /= f;
	m.d[0][1] /= f;
	m.d[1][1] /= f;
	m.d[2][1] /= f;
	m.d[3][1] /= f;
	m.d[0][2] /= f;
	m.d[1][2] /= f;
	m.d[2][2] /= f;
	m.d[3][2] /= f;
	m.d[0][3] /= f;
	m.d[1][3] /= f;
	m.d[2][3] /= f;
	m.d[3][3] /= f;
	return m;
}

bool Mat4::operator==(const Mat4& m) const {
	return (std::memcmp(this, &m, sizeof(Mat4)) == 0);
}

bool Mat4::operator!=(const Mat4& m) const {
	return (std::memcmp(this, &m, sizeof(Mat4)) != 0);
}

std::ostream& operator<<(std::ostream& out, const Mat4& m) {
	out << m.r[0];
	for (int i = 1; i < 16; ++i) {
		if (i == 4 || i == 8 || i == 12)
			out << ",";
		out << " " << m.r[i];
	}
	return out;
}

const Mat4 Mat4::identity = Mat4(1.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 1.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f);

const Mat4 Mat4::zero = Mat4(0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 0.0f);

Mat4 convert(Mat4 m, AxisDirectionConvention dst, AxisDirectionConvention src) {
	AxisDirection srcda[3] = { AxisDirection(src & 0x0003F), AxisDirection(src & 0x00FC0), AxisDirection(src & 0x3F000) };
	AxisDirection dstda[3] = { AxisDirection(dst & 0x0003F), AxisDirection(dst & 0x00FC0), AxisDirection(dst & 0x3F000) };

	Direction srcd[3] = { Direction(srcda[0] & 0x0003F), Direction((srcda[1] >> 6) & 0x0003F), Direction((srcda[2] >> 12) & 0x0003F) };
	Direction dstd[3] = { Direction(dstda[0] & 0x0003F), Direction((dstda[1] >> 6) & 0x0003F), Direction((dstda[2] >> 12) & 0x0003F) };

	Mat4 straight = Mat4::identity;
	straight.d[0][0] = 0.0f;
	straight.d[1][1] = 0.0f;
	straight.d[2][2] = 0.0f;

	Mat4 inverted = Mat4::identity;
	inverted.d[0][0] = 0.0f;
	inverted.d[1][1] = 0.0f;
	inverted.d[2][2] = 0.0f;

	for (int i = 0; i < 3; ++i) {
		Direction srcdstraight = srcd[i];
		Direction srdcinverted = (srcdstraight == R ? L : (srcdstraight == L ? R : (srcdstraight == U ? D : (srcdstraight == D ? U : (srcdstraight == B ? F : B)))));
		for (int j = 0; j < 3; ++j) {
			if (srcdstraight == dstd[j]) {
				straight.d[j][i] = +1;
				inverted.d[i][j] = +1;
			} else if (srdcinverted == dstd[j]) {
				straight.d[j][i] = -1;
				inverted.d[i][j] = -1;
			}
		}
	}

	return (straight * (m * inverted));
}
