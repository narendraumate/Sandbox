//
//  SpatialCamera.h
//
//
//  Created by Narendra Umate on 11/29/15.
//
//

#pragma once

#include "Bound.h"
#include "Frustum.h"
#include "Intersect.h"
#include "Matrix.h"
#include "Projection.h"
#include "Spatial.h"
#include "Vector.h"

enum ProjectionDirectionRestraint {
	ForwardFinite,
	ForwardInfinite,
	ReverseFinite,
	ReverseInfinite
};

class SpatialCamera : public Spatial3 {
public:
	SpatialCamera(const ProjectionType& type, const ProjectionHand& hand, const ProjectionSize& dimension, const Transform3& transform = Transform3());
	virtual ~SpatialCamera();

	void setTranslate(const Vec3& translate);
	void setRotate(const Quat& rotate);
	void setScale(const float& scale);

	void setPosition(const Vec3& position);
	void setLookAtPoint(const Vec3& lookAtPoint);
	void setAxes(const Vec3& right, const Vec3& up);
	void setFrame(const Vec3& position, const Vec3& lookAtPoint);
	void setFrame(const Vec3& position, const Quat& rotation);

	void setType(const ProjectionType& type);
	void setFrustum(const float& l, const float& r, const float& b, const float& t, const float& n, const float& f);
	void setFrustum(const float& upFovDegree, const float& aspectRatio, const float& n, const float& f);
	void setSize(const int& screenWidth, const int& screenHeight);
	void setPreViewMatrix(const Mat4& preViewMatrix);
	void setPostProjectionMatrix(const Mat4& postProjectionMatrix);

	void getSize(int& screenWidth, int& screenHeight);

	Frustum3 getFrustum() const;
	ProjectionType getProjectionType() const;
	ProjectionHand getProjectionHand() const;
	ProjectionSize getProjectionSize() const;
	ProjectionDirectionRestraint getProjectionDirectionRestraint() const;
	ProjectionParameter getProjectionParameter() const;

	Mat4 getTransform();
	Mat4 getViewMatrix();
	Mat4 getViewProjectionMatrix();
	Mat4 getProjectionMatrix();

private:
	void updateViewMatrix();
	void updateProjectionMatrix();
	void updateViewProjectionMatrix();
	void updateFrustum();

	ProjectionType m_projectionType;
	ProjectionHand m_projectionHand;
	ProjectionSize m_projectionSize;
	ProjectionDirectionRestraint m_projectionDirectionRestraint;
	ProjectionParameter m_projectionParameter;

	float m_upFovRadian;
	float m_aspectRatio;

	Mat4 m_viewMatrix;
	Mat4 m_orthographicProjectionMatrix;
	Mat4 m_perspectiveProjectionMatrix;
	Mat4 m_viewOrthographicProjectionMatrix;
	Mat4 m_viewPerspectiveProjectionMatrix;

	Frustum3 m_frustum;
};
