//
//  Intersect.h
//
//
//  Created by Narendra Umate on 12/27/16.
//
//

#pragma once

#include "Bound.h"
#include "Frustum.h"
#include "Vector.h"

bool testIntersect(const Vec3& point, const Bound3& bound);
bool testIntersect(const Vec3& point, const Frustum3& frustum);

bool testIntersect(const Bound3& bound, const Bound3& other);
bool testIntersect(const Bound3& bound, const Frustum3& frustum);
bool testIntersect(const Frustum3& frustum, const Frustum3& other);

bool boundCompletelyInsideBound(const Bound3& small, const Bound3& large);
bool boundCompletelyInsideFrustum(const Bound3& small, const Frustum3& large);
