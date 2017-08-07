//
//  Spatial.cpp
//
//
//  Created by Narendra Umate on 8/23/15.
//
//

#include "Spatial.h"

// Spatial2

Spatial2::Spatial2()
: m_worldTransform(Transform2(Mat3::identity)) {
}

Spatial2::Spatial2(const Transform2& transform)
: m_worldTransform(transform) {
}

Spatial2::~Spatial2() {
}

void Spatial2::update() {
	// Dummy
}

bool Spatial2::inside(const Bound2& bound) {
	return false; // Dummy
}

bool Spatial2::inside(const Frustum2& frustum) {
	return false; // Dummy
}

bool Spatial2::intersect(const Bound2& bound) {
	return false; // Dummy
}

bool Spatial2::intersect(const Frustum2& frustum) {
	return false; // Dummy
}

void Spatial2::setTranslateRotateScale(const Vec2& translate, const float& rotate, const Vec2& scale) {
	m_worldTransform.setTranslateRotateScale(translate, rotate, scale);
}

void Spatial2::setTranslate(const Vec2& translate) {
	m_worldTransform.setTranslate(translate);
}

void Spatial2::setRotate(const float& rotate) {
	m_worldTransform.setRotate(rotate);
}

void Spatial2::setScale(const Vec2& scale) {
	m_worldTransform.setScale(scale);
}

void Spatial2::getTranslateRotateScale(Vec2& translate, float& rotate, Vec2& scale) {
	m_worldTransform.getTranslateRotateScale(translate, rotate, scale);
}

Vec2 Spatial2::getTranslate() {
	return m_worldTransform.getTranslate();
}

float Spatial2::getRotate() {
	return m_worldTransform.getRotate();
}

Vec2 Spatial2::getScale() {
	return m_worldTransform.getScale();
}

void Spatial2::setAxes(const Vec2& up) {
	m_worldTransform.setRotate(acosf(up.y)); // acos(dot(vec2(0, 1), up))
}

void Spatial2::setFrame(const Vec2& position, const Vec2& up) {
	m_worldTransform.setTranslate(position);
	m_worldTransform.setRotate(acosf(up.y)); // acos(dot(vec2(0, 1), up))
}

void Spatial2::setPosition(const Vec2& position) {
	m_worldTransform.setTranslate(position);
}

void Spatial2::setTransform(const Transform2& transform) {
	m_worldTransform.setMatrix(transform.getMatrix());
}

void Spatial2::getAxes(Vec2& up) {
	up = m_worldTransform.getUp(true);
}

void Spatial2::getFrame(Vec2& position, Vec2& up) {
	position = m_worldTransform.getTranslate();
	up = m_worldTransform.getUp(true);
}

void Spatial2::getPosition(Vec2& position) {
	position = m_worldTransform.getTranslate();
}

void Spatial2::getTransform(Transform2& transform) {
	transform = m_worldTransform;
}

// Spatial3

Spatial3::Spatial3()
: m_worldTransform(Transform3(Mat4::identity)) {
}

Spatial3::Spatial3(const Transform3& transform)
: m_worldTransform(transform) {
}

Spatial3::~Spatial3() {
}

void Spatial3::update() {
	// Dummy
}

bool Spatial3::inside(const Bound3& bound) {
	return false; // Dummy
}

bool Spatial3::inside(const Frustum3& frustum) {
	return false; // Dummy
}

bool Spatial3::intersect(const Bound3& bound) {
	return false; // Dummy
}

bool Spatial3::intersect(const Frustum3& frustum) {
	return false; // Dummy
}

void Spatial3::setTranslateRotateScale(const Vec3& translate, const Quat& rotate, const Vec3& scale) {
	m_worldTransform.setTranslateRotateScale(translate, rotate, scale);
}

void Spatial3::setTranslate(const Vec3& translate) {
	m_worldTransform.setTranslate(translate);
}

void Spatial3::setRotate(const Quat& rotate) {
	m_worldTransform.setRotate(rotate);
}

void Spatial3::setScale(const Vec3& scale) {
	m_worldTransform.setScale(scale);
}

void Spatial3::getTranslateRotateScale(Vec3& translate, Quat& rotate, Vec3& scale) {
	m_worldTransform.getTranslateRotateScale(translate, rotate, scale);
}

Vec3 Spatial3::getTranslate() {
	return m_worldTransform.getTranslate();
}

Quat Spatial3::getRotate() {
	return m_worldTransform.getRotate();
}

Vec3 Spatial3::getScale() {
	return m_worldTransform.getScale();
}

void Spatial3::setAxes(const Vec3& right, const Vec3& up, const Vec3& back) {
	m_worldTransform.setRotate(Quat(right, up, back));
}

void Spatial3::setFrame(const Vec3& position, const Vec3& right, const Vec3& up, const Vec3& back) {
	m_worldTransform.setTranslate(position);
	m_worldTransform.setRotate(Quat(right, up, back));
}

void Spatial3::setPosition(const Vec3& position) {
	m_worldTransform.setTranslate(position);
}

void Spatial3::setTransform(const Transform3& transform) {
	m_worldTransform.setMatrix(transform.getMatrix());
}

void Spatial3::getAxes(Vec3& right, Vec3& up, Vec3& back) {
	right = m_worldTransform.getRight(true);
	up = m_worldTransform.getUp(true);
	back = m_worldTransform.getBack(true);
}

void Spatial3::getFrame(Vec3& position, Vec3& right, Vec3& up, Vec3& back) {
	position = m_worldTransform.getTranslate();
	right = m_worldTransform.getRight(true);
	up = m_worldTransform.getUp(true);
	back = m_worldTransform.getBack(true);
}

void Spatial3::getPosition(Vec3& position) {
	position = m_worldTransform.getTranslate();
}

void Spatial3::getTransform(Transform3& transform) {
	transform = m_worldTransform;
}
