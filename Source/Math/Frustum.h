//
//  Frustum.h
//
//
//  Created by Narendra Umate on 4/13/13.
//
//

#pragma once

#include "Vector.h"
#include "Projection.h"

class Frustum3 {
public:
	Frustum3();
	virtual ~Frustum3();

	void setMatrix(const ProjectionDimension& projectionDimension, const Mat4& matrixO, const bool& reverseZ);
	void set(const Mat4& viewMatrix, const ProjectionParameter& projectionParameter, const bool& infinite);

	Vec4 m_planes[6];
	Vec3 m_points[8];
};
