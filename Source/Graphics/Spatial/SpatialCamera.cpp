//
//  SpatialCamera.cpp
//
//
//  Created by Narendra Umate on 11/29/15.
//
//

#include "SpatialCamera.h"

SpatialCamera::SpatialCamera(const ProjectionType& type, const Mat4& ndc, const Transform3& transform)
: Spatial3(transform)
, m_projectionNdc(ndc)
, m_projectionType(type)
, m_projectionRange(ForwardFiniteZ)
//, m_projectionRange(ReverseInfiniteZ)
//, m_projectionRange(ReverseFiniteZ)
//, m_projectionRange(ReverseInfiniteZ)
, m_projectionParameters(-0.0849426985f,
	  +0.0849426985f,
	  -0.0637070238f,
	  +0.0637070238f,
	  +0.100f,
	  +100.0f)
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

void SpatialCamera::setScale(const Vec3& scale) {
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
	Vec3 back = position - lookAtPoint;
	back.normalize();
	Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 right = cross(up, back);
	right.normalize();
	up = cross(back, right);
	up.normalize();
	m_worldTransform.setRotate(Quat(right, up, back));
	updateViewMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setAxes(const Vec3& right, const Vec3& up, const Vec3& back) {
	m_worldTransform.setRotate(Quat(right, up, back));
	updateViewMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setFrame(const Vec3& position, const Vec3& lookAtPoint) {
	Vec3 back = position - lookAtPoint;
	back.normalize();
	Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 right = cross(up, back);
	right.normalize();
	up = cross(back, right);
	up.normalize();
	m_worldTransform.setTranslate(position);
	m_worldTransform.setRotate(Quat(right, up, back));
	updateViewMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setFrame(const Vec3& position, const Vec3& right, const Vec3& up, const Vec3& back) {
	m_worldTransform.setTranslate(position);
	m_worldTransform.setRotate(Quat(right, up, back));
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
	m_projectionParameters.l = l;
	m_projectionParameters.r = r;
	m_projectionParameters.b = b;
	m_projectionParameters.t = t;
	m_projectionParameters.n = n;
	m_projectionParameters.f = f;
	updateProjectionMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setFrustum(const float& upFovRadian, const float& aspectRatio, const float& n, const float& f) {
	m_upFovRadian = upFovRadian;
	m_aspectRatio = aspectRatio;
	m_projectionParameters.n = n;
	m_projectionParameters.f = f;
	m_projectionParameters.t = n * tanf(0.5f * upFovRadian);
	m_projectionParameters.r = aspectRatio * m_projectionParameters.t;
	m_projectionParameters.b = -m_projectionParameters.t;
	m_projectionParameters.l = -m_projectionParameters.r;
	updateProjectionMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setSize(const int& screenWidth, const int& screenHeight) {
	if (m_projectionType == Orthographic) {
		float s = m_projectionParameters.t - m_projectionParameters.b;
		float l = float(screenWidth) / float(screenHeight) / -2.0f * s;
		float r = -l;
		float b = -0.5f * s;
		float t = -b;
		setFrustum(l, r, b, t, m_projectionParameters.n, m_projectionParameters.f);
	} else if (m_projectionType == Perspective) {
		setFrustum(m_upFovRadian, float(screenWidth) / float(screenHeight), m_projectionParameters.n, m_projectionParameters.f);
	}
}

void SpatialCamera::getSize(int& screenWidth, int& screenHeight) {
	screenWidth = m_projectionParameters.r - m_projectionParameters.l;
	screenHeight = m_projectionParameters.t - m_projectionParameters.b;
}

Frustum3 SpatialCamera::getFrustum() const {
	return m_frustum;
}

Mat4 SpatialCamera::getProjectionNdc() const {
	return m_projectionNdc;
}

ProjectionType SpatialCamera::getProjectionType() const {
	return m_projectionType;
}

ProjectionRange SpatialCamera::getProjectionRange() const {
	return m_projectionRange;
}

ProjectionParameters SpatialCamera::getProjectionParameters() const {
	return m_projectionParameters;
}

Vec3 SpatialCamera::getPosition() {
	return m_worldTransform.getTranslate();
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
	// Inverse of the world transform of the camera.
	m_viewMatrix = m_worldTransform.getMatrix().inverse();
}

void SpatialCamera::updateProjectionMatrix() {
	if (m_projectionType == Orthographic) {
		m_orthographicProjectionMatrix = makeProjection(m_projectionNdc, m_projectionType, m_projectionRange, m_projectionParameters, RUB);
	} else {
		m_perspectiveProjectionMatrix = makeProjection(m_projectionNdc, m_projectionType, m_projectionRange, m_projectionParameters, RUB);
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
	if (m_projectionType == Orthographic) {
		m_frustum.setMatrix(m_viewOrthographicProjectionMatrix, m_projectionRange == ReverseFiniteZ || m_projectionRange == ReverseInfiniteZ);
	} else if (m_projectionType == Perspective) {
		m_frustum.setMatrix(m_viewPerspectiveProjectionMatrix, m_projectionRange == ReverseFiniteZ || m_projectionRange == ReverseInfiniteZ);
	}
}
