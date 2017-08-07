//
//  Intersect.cpp
//
//
//  Created by Narendra Umate on 12/27/16.
//
//

#include "Intersect.h"

bool testIntersect(const Vec3& point, const Bound3& bound) {
	return false; // TODO
}

bool testIntersect(const Vec3& point, const Frustum3& frustum) {
	return false; // TODO
}

bool testIntersect(const Bound3& bound, const Bound3& other) {
	return false; // TODO
}
/* It maybe looks like culling is broken for reverse projection */
/* Until issues are resolved test intersect will always be true */
bool testIntersect(const Bound3& bound, const Frustum3& frustum) {
	return true;
	// clang-format off
	for (unsigned int i = 0; i < 6; ++i) {
		if (dot(frustum.m_planes[i], bound.m_points[0]) < 0.0f &&
			dot(frustum.m_planes[i], bound.m_points[1]) < 0.0f &&
			dot(frustum.m_planes[i], bound.m_points[2]) < 0.0f &&
			dot(frustum.m_planes[i], bound.m_points[3]) < 0.0f &&
			dot(frustum.m_planes[i], bound.m_points[4]) < 0.0f &&
			dot(frustum.m_planes[i], bound.m_points[5]) < 0.0f &&
			dot(frustum.m_planes[i], bound.m_points[6]) < 0.0f &&
			dot(frustum.m_planes[i], bound.m_points[7]) < 0.0f) {
			return false;
		}
	}
	for (unsigned int i = 0; i < 6; ++i) {
		if (dot(bound.m_planes[i], frustum.m_points[0]) < 0.0f &&
			dot(bound.m_planes[i], frustum.m_points[1]) < 0.0f &&
			dot(bound.m_planes[i], frustum.m_points[2]) < 0.0f &&
			dot(bound.m_planes[i], frustum.m_points[3]) < 0.0f &&
			dot(bound.m_planes[i], frustum.m_points[4]) < 0.0f &&
			dot(bound.m_planes[i], frustum.m_points[5]) < 0.0f &&
			dot(bound.m_planes[i], frustum.m_points[6]) < 0.0f &&
			dot(bound.m_planes[i], frustum.m_points[7]) < 0.0f) {
			return false;
		}
	}
	return true;
	// clang-format on
}

bool testIntersect(const Frustum3& frustum, const Frustum3& other) {
	return false; // TODO
}

bool boundCompletelyInsideBound(const Bound3& small, const Bound3& large) {
	return large.contains(small);
}

bool boundCompletelyInsideFrustum(const Bound3& small, const Frustum3& large) {
	return false; // TODO
}
