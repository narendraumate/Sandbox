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

class Transform2 {
public:
	Transform2(const Mat3& matrix);
	Transform2(const Vec2& translate = Vec2::zero, const float& rotate = 0.0f, const Vec2& scale = Vec2::one);
	virtual ~Transform2();

	void setMatrix(const Mat3& matrix);
	Mat3 getMatrix() const;
	void setTranslate(const Vec2& translate);
	Vec2 getTranslate() const;
	void setRotate(const float& rotate);
	float getRotate() const;
	void setScale(const Vec2& scale);
	Vec2 getScale() const;
	void setTranslateRotateScale(const Vec2& translate, const float& rotate, const Vec2& scale);
	void getTranslateRotateScale(Vec2& translate, float& rotate, Vec2& scale) const;

	Vec2 getRight(const bool& normalize) const;
	Vec2 getUp(const bool& normalize) const;

	Transform2 operator*(const Transform2& other);
	Transform2 operator*=(const Transform2& other);

private:
	void compose();
	void decompose();

	Mat3 m_matrix;
	Vec2 m_translate;
	float m_rotate;
	Vec2 m_scale;
};

class Transform3 {
public:
	Transform3(const Mat4& matrix);
	Transform3(const Vec3& translate = Vec3::zero, const Quat& rotate = Quat::identity, const Vec3& scale = Vec3::one);
	virtual ~Transform3();

	void setMatrix(const Mat4& matrix);
	Mat4 getMatrix() const;
	void setTranslate(const Vec3& translate);
	Vec3 getTranslate() const;
	void setRotate(const Quat& rotate);
	Quat getRotate() const;
	void setScale(const Vec3& scale);
	Vec3 getScale() const;
	void setTranslateRotateScale(const Vec3& translate, const Quat& rotate, const Vec3& scale);
	void getTranslateRotateScale(Vec3& translate, Quat& rotate, Vec3& scale) const;

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
