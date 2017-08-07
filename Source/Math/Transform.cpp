//
//  Transform.cpp
//
//
//  Created by Narendra Umate on 8/24/15.
//
//

#include "Transform.h"

Transform3::Transform3(const Vec3& translate, const Quat& rotate, const float& scale)
: m_translate(translate)
, m_rotate(rotate)
, m_scale(Vec3(scale, scale, scale)) {
	compose();
	decompose();
}

Transform3::~Transform3() {
}

Mat4 Transform3::getMatrix() const {
	return m_matrix;
}

void Transform3::setTranslate(const Vec3& translate) {
	m_translate = translate;
	compose();
	decompose();
}

Vec3 Transform3::getTranslate() const {
	return m_translate;
}

void Transform3::setRotate(const Quat& rotate) {
	m_rotate = rotate;
	compose();
	decompose();
}

Quat Transform3::getRotate() const {
	return m_rotate;
}

void Transform3::setScale(const float& scale) {
	m_scale = Vec3(scale, scale, scale);
	compose();
	decompose();
}

float Transform3::getScale() const {
	return m_scale.x;
}

void Transform3::setTranslateRotateScale(const Vec3& translate, const Quat& rotate, const float& scale) {
	m_translate = translate;
	m_rotate = rotate;
	m_scale = Vec3(scale, scale, scale);
	compose();
	decompose();
}

void Transform3::getTranslateRotateScale(Vec3& translate, Quat& rotate, float& scale) const {
	translate = m_translate;
	rotate = m_rotate;
	scale = m_scale.x;
}

Vec3 Transform3::getRight(const bool& normalize) const {
	return normalize ? Vec3(m_matrix.d[0][0], m_matrix.d[1][0], m_matrix.d[2][0]).normal() : Vec3(m_matrix.d[0][0], m_matrix.d[1][0], m_matrix.d[2][0]);
}

Vec3 Transform3::getUp(const bool& normalize) const {
	return normalize ? Vec3(m_matrix.d[0][1], m_matrix.d[1][1], m_matrix.d[2][1]).normal() : Vec3(m_matrix.d[0][1], m_matrix.d[1][1], m_matrix.d[2][1]);
}

Vec3 Transform3::getBack(const bool& normalize) const {
	return normalize ? Vec3(m_matrix.d[0][2], m_matrix.d[1][2], m_matrix.d[2][2]).normal() : Vec3(m_matrix.d[0][2], m_matrix.d[1][2], m_matrix.d[2][2]);
}

Transform3 Transform3::operator*(const Transform3& other) {
	Transform3 t;
	t.m_matrix = m_matrix * other.m_matrix;
	t.decompose();
	return t;
}

Transform3 Transform3::operator*=(const Transform3& other) {
	return (*this * other);
}

void Transform3::compose() {
	// Both these paths have been tried and tested to be correct.
#if defined(UNOPTIMIZED)
	m_matrix = Mat4::translate(m_translate) * Mat4::rotate(m_rotate) * Mat4::scale(m_scale);
#else
	const float x2 = m_rotate.x + m_rotate.x;
	const float y2 = m_rotate.y + m_rotate.y;
	const float z2 = m_rotate.z + m_rotate.z;
	const float xx2 = m_rotate.x * x2;
	const float yy2 = m_rotate.y * y2;
	const float zz2 = m_rotate.z * z2;
	const float xy2 = m_rotate.x * y2;
	const float xz2 = m_rotate.x * z2;
	const float yz2 = m_rotate.y * z2;
	const float wx2 = m_rotate.w * x2;
	const float wz2 = m_rotate.w * z2;
	const float wy2 = m_rotate.w * y2;

	m_matrix.d[0][0] = (1.0f - yy2 - zz2) * m_scale.x;
	m_matrix.d[1][0] = (xy2 - wz2) * m_scale.y;
	m_matrix.d[2][0] = (xz2 + wy2) * m_scale.z;
	m_matrix.d[3][0] = m_translate.x;

	m_matrix.d[0][1] = (xy2 + wz2) * m_scale.x;
	m_matrix.d[1][1] = (1.0f - xx2 - zz2) * m_scale.y;
	m_matrix.d[2][1] = (yz2 - wx2) * m_scale.z;
	m_matrix.d[3][1] = m_translate.y;

	m_matrix.d[0][2] = (xz2 - wy2) * m_scale.x;
	m_matrix.d[1][2] = (yz2 + wx2) * m_scale.y;
	m_matrix.d[2][2] = (1.0f - xx2 - yy2) * m_scale.z;
	m_matrix.d[3][2] = m_translate.z;

	m_matrix.d[0][3] = 0.0f;
	m_matrix.d[1][3] = 0.0f;
	m_matrix.d[2][3] = 0.0f;
	m_matrix.d[3][3] = 1.0f;
#endif // UNOPTIMIZED
}

void Transform3::decompose() {
	m_scale.x = sqrtf(m_matrix.d[0][0] * m_matrix.d[0][0] + m_matrix.d[0][1] * m_matrix.d[0][1] + m_matrix.d[0][2] * m_matrix.d[0][2]);
	m_scale.y = sqrtf(m_matrix.d[1][0] * m_matrix.d[1][0] + m_matrix.d[1][1] * m_matrix.d[1][1] + m_matrix.d[1][2] * m_matrix.d[1][2]);
	m_scale.z = sqrtf(m_matrix.d[2][0] * m_matrix.d[2][0] + m_matrix.d[2][1] * m_matrix.d[2][1] + m_matrix.d[2][2] * m_matrix.d[2][2]);

	m_translate.x = m_matrix.d[3][0];
	m_translate.y = m_matrix.d[3][1];
	m_translate.z = m_matrix.d[3][2];

	if (m_scale.x == 0.0f || m_scale.y == 0.0f || m_scale.z == 0.0f) {
		m_rotate = Quat::identity;
	} else {
		Mat3 rotateMatrix = Mat3(
			m_matrix.d[0][0] / m_scale.x, m_matrix.d[0][1] / m_scale.x, m_matrix.d[0][2] / m_scale.x,
			m_matrix.d[1][0] / m_scale.y, m_matrix.d[1][1] / m_scale.y, m_matrix.d[1][2] / m_scale.y,
			m_matrix.d[2][0] / m_scale.z, m_matrix.d[2][1] / m_scale.z, m_matrix.d[2][2] / m_scale.z);
		m_rotate = Quat(rotateMatrix);
	}

	std::cout << m_translate << std::endl;
	std::cout << m_rotate << std::endl;
	std::cout << m_scale << std::endl;
	std::cout << std::endl;
}
