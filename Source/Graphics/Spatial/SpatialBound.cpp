//
//  SpatialBound.cpp
//
//
//  Created by Narendra Umate on 8/23/15.
//
//

#include "SpatialBound.h"

SpatialBound3::SpatialBound3()
: Spatial3() {
}

SpatialBound3::~SpatialBound3() {
}

void SpatialBound3::update() {
}

bool SpatialBound3::inside(const Bound3& bound) {
	return m_worldBoundingBox.completelyInside(bound);
}

bool SpatialBound3::inside(const Frustum3& frustum) {
	return m_worldBoundingBox.completelyInside(frustum);
}

bool SpatialBound3::intersect(const Bound3& bound) {
	return testIntersect(m_worldBoundingBox, bound);
}

bool SpatialBound3::intersect(const Frustum3& frustum) {
	return testIntersect(m_worldBoundingBox, frustum);
}

void SpatialBound3::setTranslate(const Vec3& translate) {
	m_worldTransform.setTranslate(translate);
	updateWorldBoundingBox();
}

void SpatialBound3::setRotate(const Quat& rotate) {
	m_worldTransform.setRotate(rotate);
	updateWorldBoundingBox();
}

void SpatialBound3::setScale(const Vec3& scale) {
	m_worldTransform.setScale(scale);
	updateWorldBoundingBox();
}

void SpatialBound3::updateWorldBoundingBox() {
	m_worldBoundingBox = m_modelBoundingBox.transform(m_worldTransform);
}
