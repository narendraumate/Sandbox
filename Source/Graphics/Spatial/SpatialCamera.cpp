//
//  SpatialCamera.cpp
//
//
//  Created by Narendra Umate on 11/29/15.
//
//

#include "SpatialCamera.h"

SpatialCamera::SpatialCamera(const ProjectionVariation& variation,
							 const ProjectionChirality& chirality,
							 const ProjectionDimension& dimension,
							 const ProjectionDirection& direction,
							 const ProjectionRestraint& restraint,
							 const Transform3& transform)
: Spatial3(transform)
, m_projectionVariation(variation)
, m_projectionChirality(chirality)
, m_projectionDimension(dimension)
, m_projectionParameter({}, {})
, m_viewMatrix(Mat4::identity)
, m_orthographicProjectionMatrix(Mat4::identity)
, m_perspectiveProjectionMatrix(Mat4::identity)
, m_viewOrthographicProjectionMatrix(Mat4::identity)
, m_viewPerspectiveProjectionMatrix(Mat4::identity) {
	switch (m_projectionVariation) {
		case Orthographic: {
			float s = 125.0f;
			float l = (4.0f / 3.0f) / -2.0f * s;
			float r = -l;
			float b = -0.5f * s;
			float t = -b;
			m_projectionParameter = {
				{ l, r, b, t },
				ProjectionParameter::makeExtent(+0.01f, +1000.0f, direction == Forward, restraint == Finite)
			};
		} break;
		case Perspective: {
			m_projectionParameter = {
				ProjectionParameter::makeTangent(120.0f * (M_PI / 180.0f), 4.0f / 3.0f),
				ProjectionParameter::makeExtent(+0.01f, +1000.0f, direction == Forward, restraint == Finite)
			};
		} break;
	}
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

void SpatialCamera::setFrustum(const Vec4& tangent, const Vec2& extent) {
	m_projectionParameter.tangent = tangent;
	m_projectionParameter.extent = extent;
	updateProjectionMatrix();
	updateViewProjectionMatrix();
	updateFrustum();
}

void SpatialCamera::setSize(const int& screenWidth, const int& screenHeight) {
	if (m_projectionVariation == Orthographic) {
		float s = m_projectionParameter.t() - m_projectionParameter.b();
		float l = float(screenWidth) / float(screenHeight) / -2.0f * s;
		float r = -l;
		float b = -0.5f * s;
		float t = -b;
		setFrustum({ l, r, b, t },
				   m_projectionParameter.extent);
	} else if (m_projectionVariation == Perspective) {
		setFrustum(ProjectionParameter::makeTangent(atan(m_projectionParameter.t() - m_projectionParameter.b()),
													float(screenWidth) / float(screenHeight)),
				   m_projectionParameter.extent);
	}
}

void SpatialCamera::getSize(int& screenWidth, int& screenHeight) {
	screenWidth = m_projectionParameter.r() - m_projectionParameter.l();
	screenHeight = m_projectionParameter.t() - m_projectionParameter.b();
}

Frustum3 SpatialCamera::getFrustum() const {
	return m_frustum;
}

ProjectionVariation SpatialCamera::getProjectionVariation() const {
	return m_projectionVariation;
}

ProjectionChirality SpatialCamera::getProjectionChirality() const {
	return m_projectionChirality;
}

ProjectionDimension SpatialCamera::getProjectionDimension() const {
	return m_projectionDimension;
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
	if (m_projectionVariation == Orthographic) {
		return m_viewOrthographicProjectionMatrix;
	} else {
		return m_viewPerspectiveProjectionMatrix;
	}
}

Mat4 SpatialCamera::getProjectionMatrix() {
	if (m_projectionVariation == Orthographic) {
		return m_orthographicProjectionMatrix;
	} else {
		return m_perspectiveProjectionMatrix;
	}
}

void SpatialCamera::updateViewMatrix() {
	m_viewMatrix = m_worldTransform.getMatrix().inverse();
}

void SpatialCamera::updateProjectionMatrix() {
	if (m_projectionVariation == Orthographic) {
		m_orthographicProjectionMatrix = Projection(m_projectionVariation,
													m_projectionChirality,
													m_projectionDimension,
													m_projectionParameter)
											 .getMatrix();
	} else {
		m_perspectiveProjectionMatrix = Projection(m_projectionVariation,
												   m_projectionChirality,
												   m_projectionDimension,
												   m_projectionParameter)
											.getMatrix();
	}
}

void SpatialCamera::updateViewProjectionMatrix() {
	if (m_projectionVariation == Orthographic) {
		m_viewOrthographicProjectionMatrix = m_orthographicProjectionMatrix * m_viewMatrix;
	} else if (m_projectionVariation == Perspective) {
		m_viewPerspectiveProjectionMatrix = m_perspectiveProjectionMatrix * m_viewMatrix;
	}
}

void SpatialCamera::updateFrustum() {
#if 0
	m_frustum.set(m_worldTransform.getMatrix(), m_projectionParameter, !m_projectionParameter.isForward());
#else
	if (m_projectionVariation == Orthographic) {
		m_frustum.setMatrix(m_projectionDimension, m_viewOrthographicProjectionMatrix, !m_projectionParameter.isForward());
	} else if (m_projectionVariation == Perspective) {
		m_frustum.setMatrix(m_projectionDimension, m_viewPerspectiveProjectionMatrix, !m_projectionParameter.isForward());
	}
#endif
}
