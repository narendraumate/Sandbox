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
#include "Vector.h"

typedef enum {
	Perspective,
	Orthographic
} ProjectionVariation;

typedef enum {
	Left,
	Right
} ProjectionChirality;

typedef enum {
	X2Y2Z1,
	X2Y2Z2
} ProjectionDimension;

typedef enum {
	Forward,
	Reverse
} ProjectionDirection;

typedef enum {
	Finite,
	Infinite
} ProjectionRestraint;

struct ProjectionParameter {
	Vec4 tangent;
	Vec2 extent;
	ProjectionParameter()
	: tangent({ 0, 0, 0, 0 })
	, extent({ 0, 0 }) {
	}
	ProjectionParameter(const Vec4& _tangent, const Vec2& _extent)
	: tangent(_tangent)
	, extent(_extent) {
	}
	float l() const {
		return tangent[0];
	}
	float r() const {
		return tangent[1];
	}
	float b() const {
		return tangent[2];
	}
	float t() const {
		return tangent[3];
	}
	float n() const {
		return fmin(extent[0], extent[1]);
	}
	float f() const {
		return fmax(extent[0], extent[1]);
	}
	bool isForward() const {
		return extent[0] < extent[1];
	}
	bool isFinite() const {
		return ((FLT_MAX)-fmax(extent[0], extent[1]) > 0.00001f);
	}
	static Vec4 makeTangent(const float& upFovRadian, const float& aspectRatio) {
		const float l = -1.0f * tan(upFovRadian * 0.5f) * aspectRatio;
		const float r = +1.0f * tan(upFovRadian * 0.5f) * aspectRatio;
		const float b = -1.0f * tan(upFovRadian * 0.5f);
		const float t = +1.0f * tan(upFovRadian * 0.5f);
		return { l, r, b, t };
	}
	static Vec2 makeExtent(const float& near, const float& far, const bool& forward, const bool& finite) {
		return {
			!forward ? (!finite ? FLT_MAX : far) : near,
			!forward ? near : (!finite ? FLT_MAX : far)
		};
	}
};

class Projection {
public:
	Projection(const ProjectionVariation& variation,
			   const ProjectionChirality& chirality,
			   const ProjectionDimension& dimension,
			   const ProjectionParameter& parameter);
	~Projection();
	Mat4 getMatrix() const;

private:
	Mat4 m_matrix;
};
