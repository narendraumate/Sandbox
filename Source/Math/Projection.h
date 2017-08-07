//
//  Projection.h
//
//
//  Created by Narendra Umate on 11/4/12.
//
//

#pragma once

#include "Convention.h"
#include "Matrix.h"
#include "Transform.h"

enum ProjectionType {
	Orthographic,
	Perspective
};

enum ProjectionRange {
	ForwardFiniteZ,
	ForwardInfiniteZ,
	ReverseFiniteZ,
	ReverseInfiniteZ
};

struct ProjectionParameters {
	float l;
	float r;
	float b;
	float t;
	float n;
	float f;

	ProjectionParameters(float _l, float _r, float _b, float _t, float _n, float _f)
	: l(_l)
	, r(_r)
	, b(_b)
	, t(_t)
	, n(_n)
	, f(_f) {
	}
};

Mat4 makeProjection(Mat4 m_projectionNdc, ProjectionType m_projectionType, ProjectionRange m_projectionRange, ProjectionParameters m_projectionParameters, AxisDirectionConvention m_axisDirectionConvention);

Mat4 warpProjection(Mat4 m_projectionNdc, ProjectionType m_projectionType, ProjectionRange m_projectionRange, ProjectionParameters m_projectionParameters, AxisDirectionConvention m_axisDirectionConvention, Mat4 oldPose, Mat4 newPose);
