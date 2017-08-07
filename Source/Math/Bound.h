//
//  Bound.h
//
//
//  Created by Narendra Umate on 8/24/15.
//
//

#pragma once

#include <cstring>
#include <list>
#include <vector>

#include "Mathematics.h"
#include "Mesh.h"
#include "Transform.h"
#include "Vector.h"

// Bound3

class Bound3 {
public:
	Bound3();
	Bound3(const Vec3& center, const Vec3& extent);
	Bound3(const std::vector<P4N4T4B4T2P2>& vertices);

	Vec3 getCenter() const;
	Vec3 getExtent() const;
	Vec3 getCornerMin() const;
	Vec3 getCornerMax() const;

	bool contains(const Vec3& point) const;
	bool contains(const Bound3& bound) const;

	Bound3 transform(const Mat4& m) const;
	Bound3 transform(const Transform3& t) const;

	P4N4T4B4T2P2 getCornerVertex(const short& octant) const;
	void getCornerVertices(std::vector<P4N4T4B4T2P2>& vertices) const;
	void getCornerIndices(std::vector<unsigned int>& indices) const;

	bool operator==(const Bound3& other) const;
	bool operator!=(const Bound3& other) const;

	Vec4 m_planes[6];
	Vec3 m_points[8];
};

Bound3 enclose(const Bound3& a, const Bound3& b);

Vec3 randomPoint(const Bound3& a);
