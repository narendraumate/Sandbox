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

class SpatialCamera : public Spatial3 {
public:
	SpatialCamera(const ProjectionType& type, const Mat4& ndc, const Transform3& transform = Transform3(Mat4::identity));
	virtual ~SpatialCamera();

	void setTranslate(const Vec3& translate);
	void setRotate(const Quat& rotate);
	void setScale(const Vec3& scale);

	void setPosition(const Vec3& position);
	void setLookAtPoint(const Vec3& lookAtPoint);
	void setAxes(const Vec3& right, const Vec3& up, const Vec3& back);
	void setFrame(const Vec3& position, const Vec3& lookAtPoint);
	void setFrame(const Vec3& position, const Vec3& right, const Vec3& up, const Vec3& back);

	void setType(const ProjectionType& type);
	void setFrustum(const float& l, const float& r, const float& b, const float& t, const float& n, const float& f);
	void setFrustum(const float& upFovDegrees, const float& aspectRatio, const float& n, const float& f);
	void setSize(const int& screenWidth, const int& screenHeight);
	void setPreViewMatrix(const Mat4& preViewMatrix);
	void setPostProjectionMatrix(const Mat4& postProjectionMatrix);

	void getSize(int& screenWidth, int& screenHeight);

	Frustum3 getFrustum() const;

	Mat4 getProjectionNdc() const;
	ProjectionType getProjectionType() const;
	ProjectionRange getProjectionRange() const;
	ProjectionParameters getProjectionParameters() const;

	Vec3 getPosition();
	Mat4 getViewMatrix();
	Mat4 getViewProjectionMatrix();
	Mat4 getProjectionMatrix();

private:
	void updateViewMatrix();
	void updateProjectionMatrix();
	void updateViewProjectionMatrix();
	void updateFrustum();

	Mat4 m_projectionNdc;
	ProjectionType m_projectionType;
	ProjectionRange m_projectionRange;
	ProjectionParameters m_projectionParameters;

	float m_upFovRadian;
	float m_aspectRatio;

	Mat4 m_viewMatrix;
	Mat4 m_orthographicProjectionMatrix;
	Mat4 m_perspectiveProjectionMatrix;
	Mat4 m_viewOrthographicProjectionMatrix;
	Mat4 m_viewPerspectiveProjectionMatrix;

	Frustum3 m_frustum;
};
