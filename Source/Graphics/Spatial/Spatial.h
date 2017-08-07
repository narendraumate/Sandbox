//
//  Spatial.h
//
//
//  Created by Narendra Umate on 8/23/15.
//
//

#pragma once

#include "Bound.h"
#include "Frustum.h"
#include "Transform.h"

class Spatial3 {
public:
	Spatial3();
	Spatial3(const Transform3& transform);
	virtual ~Spatial3() = 0;

	virtual void update();

	virtual bool inside(const Bound3& bound);
	virtual bool inside(const Frustum3& frustum);

	virtual bool intersect(const Bound3& bound);
	virtual bool intersect(const Frustum3& frustum);

	virtual void setTranslateRotateScale(const Vec3& translate, const Quat& rotate, const float& scale);
	virtual void setTranslate(const Vec3& translate);
	virtual void setRotate(const Quat& rotate);
	virtual void setScale(const float& scale);

	void getTranslateRotateScale(Vec3& translate, Quat& rotate, float& scale);
	Vec3 getTranslate();
	Quat getRotate();
	float getScale();

	void setAxes(const Vec3& right, const Vec3& up);
	void setFrame(const Vec3& position, const Quat& rotation);
	void setPosition(const Vec3& position);
	void setTransform(const Transform3& transform);

	void getAxes(Vec3& right, Vec3& up, Vec3& back);
	void getFrame(Vec3& position, Vec3& right, Vec3& up, Vec3& back);
	void getPosition(Vec3& position);
	void getTransform(Transform3& transform);

protected:
	Transform3 m_worldTransform;
};
