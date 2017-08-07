//
//  Frustum.h
//
//
//  Created by Narendra Umate on 4/13/13.
//
//

#pragma once

#include "Vector.h"

class Frustum2 {
public:
	Frustum2();
	virtual ~Frustum2();

	void setMatrix(const Mat3& matrix);

	Vec3 m_lines[4];
	Vec3 m_points[4];
};

class Frustum3 {
public:
	Frustum3();
	virtual ~Frustum3();

	void setMatrix(const Mat4& matrix, const bool& reverseZ);

	Vec4 m_planes[6];
	Vec3 m_points[8];

	enum Plane {
		r,
		l,
		t,
		b,
		n,
		f
	};

	enum Point {
		lbn,
		rbn,
		rtn,
		ltn,
		lbf,
		rbf,
		rtf,
		ltf
	};
};
