//
//  Intersect.cpp
//
//
//  Created by Narendra Umate on 12/27/16.
//
//

#include "Intersect.h"

bool testIntersect(const Vec2& point, const Vec2& other) {
	return point == other;
}

bool testIntersect(const Vec2& point, const Bound2& bound) {
	return false; // TODO
}

bool testIntersect(const Vec2& point, const Frustum2& frustum) {
	return false; // TODO
}

bool testIntersect(const Bound2& bound, const Bound2& other) {
	return false; // TODO
}

bool testIntersect(const Bound2& bound, const Frustum2& frustum) {
	return false; // TODO
}

bool testIntersect(const Frustum2& frustum, const Frustum2& other) {
	return false; // TODO
}

bool testIntersect(const Vec3& point, const Vec3& other) {
	return point == other;
}

bool testIntersect(const Vec3& point, const Bound3& bound) {
	return false; // TODO
}

bool testIntersect(const Vec3& point, const Frustum3& frustum) {
	return false; // TODO
}

bool testIntersect(const Bound3& bound, const Bound3& other) {
	return false; // TODO
}

bool testIntersect(const Bound3& bound, const Frustum3& frustum) {
	// Reference http://www.iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm

	const float band = 0.0f;

	// check box outside/inside of frustum
	for (unsigned int i = 0; i < 6; ++i) {
		int out = 0;
		out += ((dot(frustum.m_planes[i], Vec4(bound.getCornerMin().x, bound.getCornerMin().y, bound.getCornerMin().z, 1.0f)) < -band) ? 1 : 0);
		out += ((dot(frustum.m_planes[i], Vec4(bound.getCornerMax().x, bound.getCornerMin().y, bound.getCornerMin().z, 1.0f)) < -band) ? 1 : 0);
		out += ((dot(frustum.m_planes[i], Vec4(bound.getCornerMin().x, bound.getCornerMax().y, bound.getCornerMin().z, 1.0f)) < -band) ? 1 : 0);
		out += ((dot(frustum.m_planes[i], Vec4(bound.getCornerMax().x, bound.getCornerMax().y, bound.getCornerMin().z, 1.0f)) < -band) ? 1 : 0);
		out += ((dot(frustum.m_planes[i], Vec4(bound.getCornerMin().x, bound.getCornerMin().y, bound.getCornerMax().z, 1.0f)) < -band) ? 1 : 0);
		out += ((dot(frustum.m_planes[i], Vec4(bound.getCornerMax().x, bound.getCornerMin().y, bound.getCornerMax().z, 1.0f)) < -band) ? 1 : 0);
		out += ((dot(frustum.m_planes[i], Vec4(bound.getCornerMin().x, bound.getCornerMax().y, bound.getCornerMax().z, 1.0f)) < -band) ? 1 : 0);
		out += ((dot(frustum.m_planes[i], Vec4(bound.getCornerMax().x, bound.getCornerMax().y, bound.getCornerMax().z, 1.0f)) < -band) ? 1 : 0);
		if (out == 8)
			return 0;
	}

	// check frustum outside/inside box
	int out;
	out = 0;
	for (int i = 0; i < 8; ++i)
		out += ((frustum.m_points[i].x > (bound.getCornerMax().x + band)) ? 1 : 0);
	if (out == 8)
		return 0;
	out = 0;
	for (int i = 0; i < 8; ++i)
		out += ((frustum.m_points[i].x < (bound.getCornerMin().x - band)) ? 1 : 0);
	if (out == 8)
		return 0;
	out = 0;
	for (int i = 0; i < 8; ++i)
		out += ((frustum.m_points[i].y > (bound.getCornerMax().y + band)) ? 1 : 0);
	if (out == 8)
		return 0;
	out = 0;
	for (int i = 0; i < 8; ++i)
		out += ((frustum.m_points[i].y < (bound.getCornerMin().y - band)) ? 1 : 0);
	if (out == 8)
		return 0;
	out = 0;
	for (int i = 0; i < 8; ++i)
		out += ((frustum.m_points[i].z > (bound.getCornerMax().z + band)) ? 1 : 0);
	if (out == 8)
		return 0;
	out = 0;
	for (int i = 0; i < 8; ++i)
		out += ((frustum.m_points[i].z < (bound.getCornerMin().z - band)) ? 1 : 0);
	if (out == 8)
		return 0;

	return 1;
}

bool testIntersect(const Frustum3& frustum, const Frustum3& other) {
	return false; // TODO
}
