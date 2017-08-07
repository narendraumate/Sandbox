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

typedef enum {
	Perspective,
	Orthographic
} ProjectionType;

typedef enum {
	Left,
	Right
} ProjectionHand;

typedef enum {
	X2Y2Z1,
	X2Y2Z2
} ProjectionSize;

typedef enum {
	Forward,
	Reverse
} ProjectionDirection;

typedef enum {
	Finite,
	Infinite
} ProjectionRestraint;

struct ProjectionParameter {
	float lTangent;
	float rTangent;
	float bTangent;
	float tTangent;
	float n;
	float f;
	ProjectionParameter(float _l, float _r, float _b, float _t, float _n, float _f)
	: lTangent(_l)
	, rTangent(_r)
	, bTangent(_b)
	, tTangent(_t)
	, n(_n)
	, f(_f) {
	}
};

class Projection {
public:
	Projection(const ProjectionType& type,
			   const ProjectionHand& hand,
			   const ProjectionSize& size,
			   const ProjectionDirection& direction,
			   const ProjectionRestraint& restraint,
			   const ProjectionParameter& parameter);
	~Projection();
	Mat4 getMatrix() const;

private:
	Mat4 m_matrix;
};
