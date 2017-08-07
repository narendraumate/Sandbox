//
//  Transform.h
//
//
//  Created by Narendra Umate on 8/24/15.
//
//

#pragma once

#include "Matrix.h"
#include "Quaternion.h"

class Transform3 {
public:
	Transform3(const Vec3& translate = Vec3::zero, const Quat& rotate = Quat::identity, const float& scale = 1.0f);
	virtual ~Transform3();

	Mat4 getMatrix() const;

	void setTranslate(const Vec3& translate);
	Vec3 getTranslate() const;
	void setRotate(const Quat& rotate);
	Quat getRotate() const;
	void setScale(const float& scale);
	float getScale() const;
	void setTranslateRotateScale(const Vec3& translate, const Quat& rotate, const float& scale);
	void getTranslateRotateScale(Vec3& translate, Quat& rotate, float& scale) const;

	Vec3 getRight(const bool& normalize) const;
	Vec3 getUp(const bool& normalize) const;
	Vec3 getBack(const bool& normalize) const;

	Transform3 operator*(const Transform3& other);
	Transform3 operator*=(const Transform3& other);

private:
	void compose();
	void decompose();

	Mat4 m_matrix;
	Vec3 m_translate;
	Quat m_rotate;
	Vec3 m_scale;
};
