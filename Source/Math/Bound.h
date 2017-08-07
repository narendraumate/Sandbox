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

#include "Frustum.h"
#include "Mathematics.h"
#include "Mesh.h"
#include "Transform.h"
#include "Vector.h"

// Bound2

class Bound2 {
public:
	Bound2();
	Bound2(const Vec2& center, const Vec2& extent);
	Bound2(const float& minX, const float& minY, const float& maxX, const float& maxY);

	Vec2 getCenter() const {
		return m_center;
	}

	Vec2 getExtent() const {
		return m_extent;
	}

	Vec2 getCornerMin() const {
		return m_cornerMin;
	}

	Vec2 getCornerMax() const {
		return m_cornerMax;
	}

	void updateCenter(const Vec2& center);
	void updateCenterExtent(const Vec2& center, const Vec2& extent);
	void updateMinMax(const float& minX, const float& minY, const float& maxX, const float& maxY);
	void updateMinMax(const Vec2& cornerMin, const Vec2& cornerMax);

	void create(const std::list<Bound2>& boxes);
	void create(const std::list<Vec2>& points);

	bool contains(const Vec2& point) const;
	bool contains(const Bound2& bound) const;

	bool intersects(const Bound2& bound) const;

	bool completelyInside(const Bound2& bound) const;
	bool completelyInside(const Frustum2& frustum) const;

	Bound2 transform(const Mat3& m) const;
	Bound2 transform(const Transform3& t) const;

	Vec2 getCornerPosition(const short& quadrant) const;
	P2N2T2P2 getCornerVertex(const short& quadrant) const;
	void getCornerVertices(std::vector<P2N2T2P2>& vertices) const;
	void getCornerIndices(std::vector<unsigned int>& indices) const;

	bool operator==(const Bound2& other) const;
	bool operator!=(const Bound2& other) const;

private:
	Vec2 m_center;
	Vec2 m_extent;
	Vec2 m_cornerMin;
	Vec2 m_cornerMax;
};

// Bound3

class Bound3 {
public:
	Bound3();
	Bound3(const Vec3& center, const Vec3& extent);
	Bound3(const float& minX, const float& minY, const float& minZ, const float& maxX, const float& maxY, const float& maxZ);

	Vec3 getCenter() const {
		return m_center;
	}

	Vec3 getExtent() const {
		return m_extent;
	}

	Vec3 getCornerMin() const {
		return m_cornerMin;
	}

	Vec3 getCornerMax() const {
		return m_cornerMax;
	}

	void updateCenter(const Vec3& center);
	void updateCenterExtent(const Vec3& center, const Vec3& extent);
	void updateMinMax(const float& minX, const float& minY, const float& minZ, const float& maxX, const float& maxY, const float& maxZ);
	void updateMinMax(const Vec3& cornerMin, const Vec3& cornerMax);

	void create(const std::vector<Bound3>& boxes);
	void create(const std::vector<Vec3>& points);
	void create(const std::vector<P4N4T4B4T2P2>& vertices);

	bool contains(const Vec3& point) const;
	bool contains(const Bound3& bound) const;

	bool intersects(const Bound3& bound) const;

	bool completelyInside(const Bound3& bound) const;
	bool completelyInside(const Frustum3& frustum) const;

	Bound3 transform(const Mat4& m) const;
	Bound3 transform(const Transform3& t) const;

	Vec3 getCornerPosition(const short& octant) const;
	P4N4T4B4T2P2 getCornerVertex(const short& octant) const;
	void getCornerVertices(std::vector<P4N4T4B4T2P2>& vertices) const;
	void getCornerIndices(std::vector<unsigned int>& indices) const;

	bool operator==(const Bound3& other) const;
	bool operator!=(const Bound3& other) const;

private:
	Vec3 m_center;
	Vec3 m_extent;
	Vec3 m_cornerMin;
	Vec3 m_cornerMax;
};

Bound2 enclose(const Bound2& a, const Bound2& b);
Bound3 enclose(const Bound3& a, const Bound3& b);

Vec2 randomPoint(const Bound2& a);
Vec3 randomPoint(const Bound3& a);
