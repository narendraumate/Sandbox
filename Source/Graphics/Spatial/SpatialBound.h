//
//  SpatialBound.h
//
//
//  Created by Narendra Umate on 8/23/15.
//
//

#pragma once

#include "Bound.h"
#include "Frustum.h"
#include "Intersect.h"
#include "Spatial.h"
#include "Transform.h"

class SpatialBound3 : public Spatial3 {
public:
	SpatialBound3(const size_t& spatialBoundHash);
	virtual ~SpatialBound3() = 0;

	virtual void update();

	virtual bool inside(const Bound3& bound);
	virtual bool inside(const Frustum3& frustum);

	virtual bool intersect(const Bound3& bound);
	virtual bool intersect(const Frustum3& frustum);

	virtual void setTranslate(const Vec3& translate);
	virtual void setRotate(const Quat& rotate);
	virtual void setScale(const float& scale);

	size_t getSpatialBoundHash() {
		return m_spatialBoundHash;
	}

	Bound3 getModelBoundingBox() {
		return m_modelBoundingBox;
	}

	Bound3 getWorldBoundingBox() {
		return m_worldBoundingBox;
	}

protected:
	void updateWorldBoundingBox();

	size_t m_spatialBoundHash;
	Bound3 m_modelBoundingBox;
	Bound3 m_worldBoundingBox;
};
