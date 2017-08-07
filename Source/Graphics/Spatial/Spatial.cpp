//
//  Spatial.cpp
//
//
//  Created by Narendra Umate on 8/23/15.
//
//

#include "Spatial.h"

// Spatial3

Spatial3::Spatial3()
: m_worldTransform(Transform3()) {
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

void Spatial3::setTranslateRotateScale(const Vec3& translate, const Quat& rotate, const float& scale) {
	m_worldTransform.setTranslateRotateScale(translate, rotate, scale);
}

void Spatial3::setTranslate(const Vec3& translate) {
	m_worldTransform.setTranslate(translate);
}

void Spatial3::setRotate(const Quat& rotate) {
	m_worldTransform.setRotate(rotate);
}

void Spatial3::setScale(const float& scale) {
	m_worldTransform.setScale(scale);
}

void Spatial3::getTranslateRotateScale(Vec3& translate, Quat& rotate, float& scale) {
	m_worldTransform.getTranslateRotateScale(translate, rotate, scale);
}

Vec3 Spatial3::getTranslate() {
	return m_worldTransform.getTranslate();
}

Quat Spatial3::getRotate() {
	return m_worldTransform.getRotate();
}

float Spatial3::getScale() {
	return m_worldTransform.getScale();
}

void Spatial3::setAxes(const Vec3& right, const Vec3& up) {
	m_worldTransform.setRotate(rightup(right, up));
}

void Spatial3::setFrame(const Vec3& position, const Quat& rotation) {
	m_worldTransform.setTranslate(position);
	m_worldTransform.setRotate(rotation);
}

void Spatial3::setPosition(const Vec3& position) {
	m_worldTransform.setTranslate(position);
}

void Spatial3::setTransform(const Transform3& transform) {
	m_worldTransform = transform;
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
