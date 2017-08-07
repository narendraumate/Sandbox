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

class Spatial2 {
public:
	Spatial2();
	Spatial2(const Transform2& transform);
	virtual ~Spatial2() = 0;

	virtual void update();

	virtual bool inside(const Bound2& bound);
	virtual bool inside(const Frustum2& frustum);

	virtual bool intersect(const Bound2& bound);
	virtual bool intersect(const Frustum2& frustum);

	virtual void setTranslateRotateScale(const Vec2& translate, const float& rotate, const Vec2& scale);
	virtual void setTranslate(const Vec2& translate);
	virtual void setRotate(const float& rotate);
	virtual void setScale(const Vec2& scale);

	void getTranslateRotateScale(Vec2& translate, float& rotate, Vec2& scale);
	Vec2 getTranslate();
	float getRotate();
	Vec2 getScale();

	void setAxes(const Vec2& up);
	void setFrame(const Vec2& position, const Vec2& up);
	void setPosition(const Vec2& position);
	void setTransform(const Transform2& transform);

	void getAxes(Vec2& up);
	void getFrame(Vec2& position, Vec2& up);
	void getPosition(Vec2& position);
	void getTransform(Transform2& transform);

protected:
	Transform2 m_worldTransform;
};

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

	virtual void setTranslateRotateScale(const Vec3& translate, const Quat& rotate, const Vec3& scale);
	virtual void setTranslate(const Vec3& translate);
	virtual void setRotate(const Quat& rotate);
	virtual void setScale(const Vec3& scale);

	void getTranslateRotateScale(Vec3& translate, Quat& rotate, Vec3& scale);
	Vec3 getTranslate();
	Quat getRotate();
	Vec3 getScale();

	void setAxes(const Vec3& right, const Vec3& up, const Vec3& back);
	void setFrame(const Vec3& position, const Vec3& right, const Vec3& up, const Vec3& back);
	void setPosition(const Vec3& position);
	void setTransform(const Transform3& transform);

	void getAxes(Vec3& right, Vec3& up, Vec3& back);
	void getFrame(Vec3& position, Vec3& right, Vec3& up, Vec3& back);
	void getPosition(Vec3& position);
	void getTransform(Transform3& transform);

protected:
	Transform3 m_worldTransform;
};
