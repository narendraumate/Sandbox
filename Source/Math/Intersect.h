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

bool testIntersect(const Vec2& point, const Vec2& other);
bool testIntersect(const Vec2& point, const Bound2& bound);
bool testIntersect(const Vec2& point, const Frustum2& frustum);

bool testIntersect(const Bound2& bound, const Bound2& other);
bool testIntersect(const Bound2& bound, const Frustum2& frustum);
bool testIntersect(const Frustum2& frustum, const Frustum2& other);

bool testIntersect(const Vec3& point, const Vec3& other);
bool testIntersect(const Vec3& point, const Bound3& bound);
bool testIntersect(const Vec3& point, const Frustum3& frustum);

bool testIntersect(const Bound3& bound, const Bound3& other);
bool testIntersect(const Bound3& bound, const Frustum3& frustum);
bool testIntersect(const Frustum3& frustum, const Frustum3& other);
