//
//  SpatialCamera.cpp
//
//
//  Created by Narendra Umate on 11/29/15.
//
//

#include "SpatialCamera.h"

SpatialCamera::SpatialCamera(const ProjectionType& type, const ProjectionHand& hand, const ProjectionSize& dimension, const Transform3& transform)
: Spatial3(transform)
, m_projectionType(type)
, m_projectionHand(hand)
, m_projectionSize(dimension)
, m_projectionDirectionRestraint(ForwardFinite)
//, m_projectionDirectionRestraint(ForwardInfinite)
//, m_projectionDirectionRestraint(ReverseFinite)
//, m_projectionDirectionRestraint(ReverseInfinite)
, m_projectionParameter(-2.30940127,
						+2.30940127,
						-1.7320509,
						+1.7320509,
						+0.01f,
						+1000.0f)
, m_upFovRadian(65.0f * (M_PI / 180.0f))
, m_aspectRatio(4.0f / 3.0f)
, m_viewMatrix(Mat4::identity)
, m_orthographicProjectionMatrix(Mat4::identity)
, m_perspectiveProjectionMatrix(Mat4::identity)
, m_viewOrthographicProjectionMatrix(Mat4::identity)
, m_viewPerspectiveProjectionMatrix(Mat4::identity) {
	updateViewMatrix();
	updateProjectionMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

SpatialCamera::~SpatialCamera() {
}

void SpatialCamera::setTranslate(const Vec3& translate) {
	m_worldTransform.setTranslate(translate);
	updateViewMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setRotate(const Quat& rotate) {
	m_worldTransform.setRotate(rotate);
	updateViewMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setScale(const float& scale) {
	m_worldTransform.setScale(scale);
	updateViewMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setPosition(const Vec3& position) {
	m_worldTransform.setTranslate(position);
	updateViewMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setLookAtPoint(const Vec3& lookAtPoint) {
	Vec3 position = m_worldTransform.getTranslate();
	Vec3 direction = lookAtPoint - position;
	Vec3 back = -direction.normal();
	Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 right = cross(up, back).normal();
	up = cross(back, right).normal();
	m_worldTransform.setRotate(rightupback(right, up, back));
	updateViewMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setAxes(const Vec3& right, const Vec3& up) {
	m_worldTransform.setRotate(rightup(right, up));
	updateViewMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setFrame(const Vec3& position, const Vec3& lookAtPoint) {
	Vec3 direction = lookAtPoint - position;
	Vec3 back = -direction.normal();
	Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 right = cross(up, back).normal();
	up = cross(back, right).normal();
	m_worldTransform.setTranslate(position);
	m_worldTransform.setRotate(rightupback(right, up, back));
	updateViewMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setFrame(const Vec3& position, const Quat& rotation) {
	m_worldTransform.setTranslate(position);
	m_worldTransform.setRotate(rotation);
	updateViewMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setType(const ProjectionType& type) {
	m_projectionType = type;
	updateProjectionMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setFrustum(const float& l, const float& r, const float& b, const float& t, const float& n, const float& f) {
	m_projectionParameter.lTangent = l;
	m_projectionParameter.rTangent = r;
	m_projectionParameter.bTangent = b;
	m_projectionParameter.tTangent = t;
	m_projectionParameter.n = n;
	m_projectionParameter.f = f;
	updateProjectionMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setFrustum(const float& upFovDegree, const float& aspectRatio, const float& n, const float& f) {
	m_upFovRadian = upFovDegree * (M_PI / 180.0f);
	m_aspectRatio = aspectRatio;

	m_projectionParameter.n = n;
	m_projectionParameter.f = f;

	m_projectionParameter.tTangent = tanf(0.5f * m_upFovRadian);
	m_projectionParameter.rTangent = tanf(0.5f * m_upFovRadian) * aspectRatio;

#if 0 // SUBDIVIDE
	m_projectionParameter.tTangent *= 0.5f;
	m_projectionParameter.rTangent *= 0.5f;
#endif

	m_projectionParameter.bTangent = -m_projectionParameter.tTangent;
	m_projectionParameter.lTangent = -m_projectionParameter.rTangent;

	updateProjectionMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setSize(const int& screenWidth, const int& screenHeight) {
	if (m_projectionType == Orthographic) {
		float s = m_projectionParameter.tTangent - m_projectionParameter.bTangent;
		float l = float(screenWidth) / float(screenHeight) / -2.0f * s;
		float r = -l;
		float b = -0.5f * s;
		float t = -b;
		setFrustum(l, r, b, t, m_projectionParameter.n, m_projectionParameter.f);
	} else if (m_projectionType == Perspective) {
		setFrustum(m_upFovRadian, float(screenWidth) / float(screenHeight), m_projectionParameter.n, m_projectionParameter.f);
	}
}

void SpatialCamera::getSize(int& screenWidth, int& screenHeight) {
	screenWidth = m_projectionParameter.rTangent - m_projectionParameter.lTangent;
	screenHeight = m_projectionParameter.tTangent - m_projectionParameter.bTangent;
}

Frustum3 SpatialCamera::getFrustum() const {
	return m_frustum;
}

ProjectionType SpatialCamera::getProjectionType() const {
	return m_projectionType;
}

ProjectionHand SpatialCamera::getProjectionHand() const {
	return m_projectionHand;
}

ProjectionSize SpatialCamera::getProjectionSize() const {
	return m_projectionSize;
}

ProjectionDirectionRestraint SpatialCamera::getProjectionDirectionRestraint() const {
	return m_projectionDirectionRestraint;
}

ProjectionParameter SpatialCamera::getProjectionParameter() const {
	return m_projectionParameter;
}

Mat4 SpatialCamera::getTransform() {
	return m_worldTransform.getMatrix();
}

Mat4 SpatialCamera::getViewMatrix() {
	return m_viewMatrix;
}

Mat4 SpatialCamera::getViewProjectionMatrix() {
	if (m_projectionType == Orthographic) {
		return m_viewOrthographicProjectionMatrix;
	} else {
		return m_viewPerspectiveProjectionMatrix;
	}
}

Mat4 SpatialCamera::getProjectionMatrix() {
	if (m_projectionType == Orthographic) {
		return m_orthographicProjectionMatrix;
	} else {
		return m_perspectiveProjectionMatrix;
	}
}

void SpatialCamera::updateViewMatrix() {
	m_viewMatrix = m_worldTransform.getMatrix().inverse();
}

void SpatialCamera::updateProjectionMatrix() {
	ProjectionDirection m_direction;
	ProjectionRestraint m_range;

	switch (m_projectionDirectionRestraint) {
		case ForwardFinite: {
			m_direction = Forward;
			m_range = Finite;
		} break;

		case ForwardInfinite: {
			m_direction = Forward;
			m_range = Infinite;
		} break;

		case ReverseFinite: {
			m_direction = Reverse;
			m_range = Finite;
		} break;

		default:
		case ReverseInfinite: {
			m_direction = Reverse;
			m_range = Infinite;
		} break;
	}

	if (m_projectionType == Orthographic) {
		m_orthographicProjectionMatrix = Projection(m_projectionType, m_projectionHand, m_projectionSize, m_direction, m_range, m_projectionParameter).getMatrix();
	} else {
		m_perspectiveProjectionMatrix = Projection(m_projectionType, m_projectionHand, m_projectionSize, m_direction, m_range, m_projectionParameter).getMatrix();
	}
}

void SpatialCamera::updateViewProjectionMatrix() {
	if (m_projectionType == Orthographic) {
		m_viewOrthographicProjectionMatrix = m_orthographicProjectionMatrix * m_viewMatrix;
	} else if (m_projectionType == Perspective) {
		m_viewPerspectiveProjectionMatrix = m_perspectiveProjectionMatrix * m_viewMatrix;
	}
}

void SpatialCamera::updateFrustum() {
#if 0
	m_frustum.set(m_worldTransform.getMatrix(), m_projectionParameter, m_projectionDirectionRestraint == ForwardInfinite || m_projectionDirectionRestraint == ReverseInfinite);
#else
	if (m_projectionType == Orthographic) {
		m_frustum.setMatrix(m_projectionSize, m_viewOrthographicProjectionMatrix, m_projectionDirectionRestraint == ReverseFinite || m_projectionDirectionRestraint == ReverseInfinite);
	} else if (m_projectionType == Perspective) {
		m_frustum.setMatrix(m_projectionSize, m_viewPerspectiveProjectionMatrix, m_projectionDirectionRestraint == ReverseFinite || m_projectionDirectionRestraint == ReverseInfinite);
	}
#endif
}
