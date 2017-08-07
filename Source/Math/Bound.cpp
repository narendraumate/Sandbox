//
//  Bound.cpp
//
//
//  Created by Narendra Umate on 8/24/15.
//
//

#include "Bound.h"

// Bound2

Bound2::Bound2()
: m_center(Vec2::zero)
, m_extent(Vec2::zero) {
}

Bound2::Bound2(const Vec2& center, const Vec2& extent)
: m_center(center)
, m_extent(extent) {
	m_cornerMin = center - extent;
	m_cornerMax = center + extent;
}

Bound2::Bound2(const float& minX, const float& minY, const float& maxX, const float& maxY) {
	m_cornerMin.x = minX;
	m_cornerMin.y = minY;
	m_cornerMax.x = maxX;
	m_cornerMax.y = maxY;
	m_center = (m_cornerMin + m_cornerMax) * 0.5f;
	m_extent = m_center - m_cornerMin;
}

void Bound2::updateCenter(const Vec2& center) {
	m_center = center;
	m_cornerMin = center - m_extent;
	m_cornerMax = center + m_extent;
}

void Bound2::updateCenterExtent(const Vec2& center, const Vec2& extent) {
	m_center = center;
	m_extent = extent;
	m_cornerMin = center - extent;
	m_cornerMax = center + extent;
}

void Bound2::updateMinMax(const Vec2& cornerMin, const Vec2& cornerMax) {
	m_cornerMin = cornerMin;
	m_cornerMax = cornerMax;
	m_center = (m_cornerMin + m_cornerMax) * 0.5f;
	m_extent = m_center - m_cornerMin;
}

void Bound2::updateMinMax(const float& minX, const float& minY, const float& maxX, const float& maxY) {
	m_cornerMin.x = minX;
	m_cornerMin.y = minY;
	m_cornerMax.x = maxX;
	m_cornerMax.y = maxY;
	m_center = (m_cornerMin + m_cornerMax) * 0.5f;
	m_extent = m_center - m_cornerMin;
}

void Bound2::create(const std::list<Bound2>& boxes) {
	std::list<Vec2> points;
	for (std::list<Bound2>::const_iterator boxPointer = boxes.begin(); boxPointer != boxes.end(); ++boxPointer) {
		// min
		Vec2 min = boxPointer->getCornerMin();
		if (min.x < m_cornerMin.x) {
			m_cornerMin.x = min.x;
		}

		if (min.x > m_cornerMax.x) {
			m_cornerMax.x = min.x;
		}

		if (min.y < m_cornerMin.y) {
			m_cornerMin.y = min.y;
		}

		if (min.y > m_cornerMax.y) {
			m_cornerMax.y = min.y;
		}
		// max
		Vec2 max = boxPointer->getCornerMax();
		if (max.x < m_cornerMin.x) {
			m_cornerMin.x = max.x;
		}

		if (max.x > m_cornerMax.x) {
			m_cornerMax.x = max.x;
		}

		if (max.y < m_cornerMin.y) {
			m_cornerMin.y = max.y;
		}

		if (max.y > m_cornerMax.y) {
			m_cornerMax.y = max.y;
		}
	}
	m_center = (m_cornerMin + m_cornerMax) * 0.5f;
	m_extent = m_center - m_cornerMin;
}

void Bound2::create(const std::list<Vec2>& points) {
	m_cornerMin = Vec2::max;
	m_cornerMax = Vec2::min;
	for (std::list<Vec2>::const_iterator pointPointer = points.begin(); pointPointer != points.end(); ++pointPointer) {
		if (pointPointer->x < m_cornerMin.x) {
			m_cornerMin.x = pointPointer->x;
		}

		if (pointPointer->x > m_cornerMax.x) {
			m_cornerMax.x = pointPointer->x;
		}

		if (pointPointer->y < m_cornerMin.y) {
			m_cornerMin.y = pointPointer->y;
		}

		if (pointPointer->y > m_cornerMax.y) {
			m_cornerMax.y = pointPointer->y;
		}
	}
	m_center = (m_cornerMin + m_cornerMax) * 0.5f;
	m_extent = m_center - m_cornerMin;
}

bool Bound2::contains(const Vec2& point) const {
	if (point.x > m_cornerMax.x) {
		return false;
	}

	if (point.x < m_cornerMin.x) {
		return false;
	}

	if (point.y > m_cornerMax.y) {
		return false;
	}

	if (point.y < m_cornerMin.y) {
		return false;
	}

	return true;
}

bool Bound2::contains(const Bound2& bound) const {
	return (contains(bound.m_cornerMin) && contains(bound.m_cornerMax));
}

bool Bound2::intersects(const Bound2& bound) const {
	// Calculate current and min non intersecting distance between centers.
	// If we are not intersecting at all then return (0, 0).
	Vec2 curDistance = m_center - bound.m_center;
	Vec2 minDistance = m_extent + bound.m_extent;
	return ((!(fabsf(curDistance.x) >= minDistance.x)) && (!(fabsf(curDistance.y) >= minDistance.y)));
}

bool Bound2::completelyInside(const Bound2& bound) const {
	return bound.contains(*this);
}

bool Bound2::completelyInside(const Frustum2& frustum) const {
	// TODO
	return true;
}

Bound2 Bound2::transform(const Mat3& m) const {
#if defined UNOPTIMIZED
	Bound boundingBox;
	Vec2 min = Vec2::max;
	Vec2 max = Vec2::min;
	for (unsigned int i = 0; i < 8; ++i) {
		Vec4 p4 = pointVector(m_center + entrywiseProduct(m_extent, getDirection(i))) * m;
		Vec2 p3 = Vec2(p4.x, p4.y, p4.z);
		min = minVec(min, p3);
		max = maxVec(max, p3);
	}
	boundingBox.updateMinMax(min, max);
	return boundingBox;
#else
	Bound2 boundingBox;
	Vec2 xa = Transform2(m).getRight(false) * m_cornerMin.x;
	Vec2 xb = Transform2(m).getRight(false) * m_cornerMax.x;
	Vec2 ya = Transform2(m).getUp(false) * m_cornerMin.y;
	Vec2 yb = Transform2(m).getUp(false) * m_cornerMax.y;
	Vec2 min = minVec(xa, xb) + minVec(ya, yb) + Transform2(m).getTranslate();
	Vec2 max = maxVec(xa, xb) + maxVec(ya, yb) + Transform2(m).getTranslate();
	boundingBox.updateMinMax(min, max);
	return boundingBox;
#endif
}

Bound2 Bound2::transform(const Transform3& t) const {
	return transform(t);
}

Vec2 Bound2::getCornerPosition(const short& quadrant) const {
	switch (quadrant % 4) {
		case 0:
			return Vec2(-1, -1) * m_extent + m_center;
		case 1:
			return Vec2(-1, +1) * m_extent + m_center;
		case 2:
			return Vec2(+1, -1) * m_extent + m_center;
		case 3:
		default:
			return Vec2(+1, +1) * m_extent + m_center;
	}
}

P2N2T2P2 Bound2::getCornerVertex(const short& quadrant) const {
	switch (quadrant % 4) {
		case 0:
			return P2N2T2P2(Vec2(-1, -1) * m_extent + m_center, Vec2(-1, -1), Vec2(0, 0));
		case 1:
			return P2N2T2P2(Vec2(-1, +1) * m_extent + m_center, Vec2(-1, +1), Vec2(0, 1));
		case 2:
			return P2N2T2P2(Vec2(+1, -1) * m_extent + m_center, Vec2(+1, -1), Vec2(1, 0));
		case 3:
		default:
			return P2N2T2P2(Vec2(+1, +1) * m_extent + m_center, Vec2(+1, +1), Vec2(1, 1));
	}
}

void Bound2::getCornerVertices(std::vector<P2N2T2P2>& vertices) const {
	vertices.resize(4);
	vertices[0] = P2N2T2P2(getCornerVertex(0));
	vertices[1] = P2N2T2P2(getCornerVertex(1));
	vertices[2] = P2N2T2P2(getCornerVertex(2));
	vertices[3] = P2N2T2P2(getCornerVertex(3));
}

void Bound2::getCornerIndices(std::vector<unsigned int>& indices) const {
	indices.resize(8);
	// TODO Verify if winding order of vertices here works.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 1;
	indices[3] = 3;
	indices[4] = 3;
	indices[5] = 2;
	indices[6] = 2;
	indices[7] = 0;
}

bool Bound2::operator==(const Bound2& other) const {
	return (std::memcmp(this, &other, sizeof(Bound2)) == 0);
}

bool Bound2::operator!=(const Bound2& other) const {
	return (std::memcmp(this, &other, sizeof(Bound2)) != 0);
}

// Bound3

Bound3::Bound3()
: m_center(Vec3::zero)
, m_extent(Vec3::zero) {
}

Bound3::Bound3(const Vec3& center, const Vec3& extent)
: m_center(center)
, m_extent(extent) {
	m_cornerMin = center - extent;
	m_cornerMax = center + extent;
}

Bound3::Bound3(const float& minX, const float& minY, const float& minZ, const float& maxX, const float& maxY, const float& maxZ) {
	m_cornerMin.x = minX;
	m_cornerMin.y = minY;
	m_cornerMin.z = minZ;
	m_cornerMax.x = maxX;
	m_cornerMax.y = maxY;
	m_cornerMax.z = maxZ;
	m_center = (m_cornerMin + m_cornerMax) * 0.5f;
	m_extent = m_center - m_cornerMin;
}

void Bound3::updateCenter(const Vec3& center) {
	m_center = center;
	m_cornerMin = center - m_extent;
	m_cornerMax = center + m_extent;
}

void Bound3::updateCenterExtent(const Vec3& center, const Vec3& extent) {
	m_center = center;
	m_extent = extent;
	m_cornerMin = center - extent;
	m_cornerMax = center + extent;
}

void Bound3::updateMinMax(const Vec3& cornerMin, const Vec3& cornerMax) {
	m_cornerMin = cornerMin;
	m_cornerMax = cornerMax;
	m_center = (m_cornerMin + m_cornerMax) * 0.5f;
	m_extent = m_center - m_cornerMin;
}

void Bound3::updateMinMax(const float& minX, const float& minY, const float& minZ, const float& maxX, const float& maxY, const float& maxZ) {
	m_cornerMin.x = minX;
	m_cornerMin.y = minY;
	m_cornerMin.z = minZ;
	m_cornerMax.x = maxX;
	m_cornerMax.y = maxY;
	m_cornerMax.z = maxZ;
	m_center = (m_cornerMin + m_cornerMax) * 0.5f;
	m_extent = m_center - m_cornerMin;
}

void Bound3::create(const std::vector<Bound3>& boxes) {
	std::vector<Vec3> points;
	for (std::vector<Bound3>::const_iterator boxPointer = boxes.begin(); boxPointer != boxes.end(); ++boxPointer) {
		// min
		Vec3 min = boxPointer->getCornerMin();
		if (min.x < m_cornerMin.x) {
			m_cornerMin.x = min.x;
		}

		if (min.x > m_cornerMax.x) {
			m_cornerMax.x = min.x;
		}

		if (min.y < m_cornerMin.y) {
			m_cornerMin.y = min.y;
		}

		if (min.y > m_cornerMax.y) {
			m_cornerMax.y = min.y;
		}

		if (min.z < m_cornerMin.z) {
			m_cornerMin.z = min.z;
		}

		if (min.z > m_cornerMax.z) {
			m_cornerMax.z = min.z;
		}
		// max
		Vec3 max = boxPointer->getCornerMax();
		if (max.x < m_cornerMin.x) {
			m_cornerMin.x = max.x;
		}

		if (max.x > m_cornerMax.x) {
			m_cornerMax.x = max.x;
		}

		if (max.y < m_cornerMin.y) {
			m_cornerMin.y = max.y;
		}

		if (max.y > m_cornerMax.y) {
			m_cornerMax.y = max.y;
		}

		if (max.z < m_cornerMin.z) {
			m_cornerMin.z = max.z;
		}

		if (max.z > m_cornerMax.z) {
			m_cornerMax.z = max.z;
		}
	}
	m_center = (m_cornerMin + m_cornerMax) * 0.5f;
	m_extent = m_center - m_cornerMin;
}

void Bound3::create(const std::vector<Vec3>& points) {
	m_cornerMin = Vec3::max;
	m_cornerMax = Vec3::min;
	for (std::vector<Vec3>::const_iterator pointPointer = points.begin(); pointPointer != points.end(); ++pointPointer) {
		if (pointPointer->x < m_cornerMin.x) {
			m_cornerMin.x = pointPointer->x;
		}

		if (pointPointer->x > m_cornerMax.x) {
			m_cornerMax.x = pointPointer->x;
		}

		if (pointPointer->y < m_cornerMin.y) {
			m_cornerMin.y = pointPointer->y;
		}

		if (pointPointer->y > m_cornerMax.y) {
			m_cornerMax.y = pointPointer->y;
		}

		if (pointPointer->z < m_cornerMin.z) {
			m_cornerMin.z = pointPointer->z;
		}

		if (pointPointer->z > m_cornerMax.z) {
			m_cornerMax.z = pointPointer->z;
		}
	}
	m_center = (m_cornerMin + m_cornerMax) * 0.5f;
	m_extent = m_center - m_cornerMin;
}

void Bound3::create(const std::vector<P4N4T4B4T2P2>& vertices) {
	m_cornerMin = Vec3::max;
	m_cornerMax = Vec3::min;
	for (std::vector<P4N4T4B4T2P2>::const_iterator vertexPointer = vertices.begin(); vertexPointer != vertices.end(); ++vertexPointer) {
		if (vertexPointer->position.x < m_cornerMin.x) {
			m_cornerMin.x = vertexPointer->position.x;
		}

		if (vertexPointer->position.x > m_cornerMax.x) {
			m_cornerMax.x = vertexPointer->position.x;
		}

		if (vertexPointer->position.y < m_cornerMin.y) {
			m_cornerMin.y = vertexPointer->position.y;
		}

		if (vertexPointer->position.y > m_cornerMax.y) {
			m_cornerMax.y = vertexPointer->position.y;
		}

		if (vertexPointer->position.z < m_cornerMin.z) {
			m_cornerMin.z = vertexPointer->position.z;
		}

		if (vertexPointer->position.z > m_cornerMax.z) {
			m_cornerMax.z = vertexPointer->position.z;
		}
	}
	m_center = (m_cornerMin + m_cornerMax) * 0.5f;
	m_extent = m_center - m_cornerMin;
}

bool Bound3::contains(const Vec3& point) const {
	if (point.x > m_cornerMax.x) {
		return false;
	}

	if (point.x < m_cornerMin.x) {
		return false;
	}

	if (point.y > m_cornerMax.y) {
		return false;
	}

	if (point.y < m_cornerMin.y) {
		return false;
	}

	if (point.z > m_cornerMax.z) {
		return false;
	}

	if (point.z < m_cornerMin.z) {
		return false;
	}

	return true;
}

bool Bound3::contains(const Bound3& bound) const {
	return (contains(bound.m_cornerMin) && contains(bound.m_cornerMax));
}

bool Bound3::intersects(const Bound3& bound) const {
	// Calculate current and min non intersecting distance between centers.
	// If we are not intersecting at all then return (0, 0, 0).
	Vec3 curDistance = m_center - bound.m_center;
	Vec3 minDistance = m_extent + bound.m_extent;
	return ((!(fabsf(curDistance.x) >= minDistance.x)) && (!(fabsf(curDistance.y) >= minDistance.y)) && (!(fabsf(curDistance.z) >= minDistance.z)));
}

bool Bound3::completelyInside(const Bound3& bound) const {
	return bound.contains(*this);
}

bool Bound3::completelyInside(const Frustum3& frustum) const {
	return false; // TODO
}

Bound3 Bound3::transform(const Mat4& m) const {
//#define UNOPTIMIZED
#if defined UNOPTIMIZED
	Vec3 min = Vec3::max;
	Vec3 max = Vec3::min;
	for (unsigned int i = 0; i < 8; ++i) {
		// This order of multiplication between matrix and vector is correct and verified.
		Vec4 p4 = m * pointVector(m_center + entrywiseProduct(m_extent, getDirection(i)));
		Vec3 p3 = Vec3(p4.x, p4.y, p4.z);
		min = minVec(min, p3);
		max = maxVec(max, p3);
	}
	Bound3 boundingBox;
	boundingBox.updateMinMax(min, max);
	return boundingBox;
#else
	Vec3 rr = Transform3(m).getRight(false);
	Vec3 uu = Transform3(m).getUp(false);
	Vec3 bb = Transform3(m).getBack(false);
	Vec3 xa = rr * m_cornerMin.x;
	Vec3 xb = rr * m_cornerMax.x;
	Vec3 ya = uu * m_cornerMin.y;
	Vec3 yb = uu * m_cornerMax.y;
	Vec3 za = bb * m_cornerMin.z;
	Vec3 zb = bb * m_cornerMax.z;
	Vec3 min = minVec(xa, xb) + minVec(ya, yb) + minVec(za, zb) + Transform3(m).getTranslate();
	Vec3 max = maxVec(xa, xb) + maxVec(ya, yb) + maxVec(za, zb) + Transform3(m).getTranslate();
	Bound3 boundingBox;
	boundingBox.updateMinMax(min, max);
	return boundingBox;
#endif
	//#undef UNOPTIMIZED
}

Bound3 Bound3::transform(const Transform3& t) const {
	return transform(t.getMatrix());
}

Vec3 Bound3::getCornerPosition(const short& octant) const {
	switch (octant % 8) {
		case 0:
			return Vec3(-1, -1, -1) * m_extent + m_center;
		case 1:
			return Vec3(-1, -1, +1) * m_extent + m_center;
		case 2:
			return Vec3(-1, +1, -1) * m_extent + m_center;
		case 3:
			return Vec3(-1, +1, +1) * m_extent + m_center;
		case 4:
			return Vec3(+1, -1, -1) * m_extent + m_center;
		case 5:
			return Vec3(+1, -1, +1) * m_extent + m_center;
		case 6:
			return Vec3(+1, +1, -1) * m_extent + m_center;
		case 7:
		default:
			return Vec3(+1, +1, +1) * m_extent + m_center;
	}
}

P4N4T4B4T2P2 Bound3::getCornerVertex(const short& octant) const {
	switch (octant % 8) {
		case 0:
			return P4N4T4B4T2P2(Vec3(-1, -1, -1) * m_extent + m_center, Vec3(-1, -1, -1), Vec2(0, 0));
		case 1:
			return P4N4T4B4T2P2(Vec3(-1, -1, +1) * m_extent + m_center, Vec3(-1, -1, +1), Vec2(0, 1));
		case 2:
			return P4N4T4B4T2P2(Vec3(-1, +1, -1) * m_extent + m_center, Vec3(-1, +1, -1), Vec2(1, 0));
		case 3:
			return P4N4T4B4T2P2(Vec3(-1, +1, +1) * m_extent + m_center, Vec3(-1, +1, +1), Vec2(1, 1));
		case 4:
			return P4N4T4B4T2P2(Vec3(+1, -1, -1) * m_extent + m_center, Vec3(+1, -1, -1), Vec2(0, 0));
		case 5:
			return P4N4T4B4T2P2(Vec3(+1, -1, +1) * m_extent + m_center, Vec3(+1, -1, +1), Vec2(0, 1));
		case 6:
			return P4N4T4B4T2P2(Vec3(+1, +1, -1) * m_extent + m_center, Vec3(+1, +1, -1), Vec2(1, 0));
		case 7:
		default:
			return P4N4T4B4T2P2(Vec3(+1, +1, +1) * m_extent + m_center, Vec3(+1, +1, +1), Vec2(1, 1));
	}
}

void Bound3::getCornerVertices(std::vector<P4N4T4B4T2P2>& vertices) const {
	vertices.resize(8);
	vertices[0] = P4N4T4B4T2P2(getCornerVertex(0));
	vertices[1] = P4N4T4B4T2P2(getCornerVertex(1));
	vertices[2] = P4N4T4B4T2P2(getCornerVertex(2));
	vertices[3] = P4N4T4B4T2P2(getCornerVertex(3));
	vertices[4] = P4N4T4B4T2P2(getCornerVertex(4));
	vertices[5] = P4N4T4B4T2P2(getCornerVertex(5));
	vertices[6] = P4N4T4B4T2P2(getCornerVertex(6));
	vertices[7] = P4N4T4B4T2P2(getCornerVertex(7));
}

void Bound3::getCornerIndices(std::vector<unsigned int>& indices) const {
	indices.resize(24);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 1;
	indices[3] = 3;
	indices[4] = 3;
	indices[5] = 2;
	indices[6] = 2;
	indices[7] = 0;
	indices[8] = 4;
	indices[9] = 5;
	indices[10] = 5;
	indices[11] = 7;
	indices[12] = 7;
	indices[13] = 6;
	indices[14] = 6;
	indices[15] = 4;
	indices[16] = 0;
	indices[17] = 4;
	indices[18] = 1;
	indices[19] = 5;
	indices[20] = 2;
	indices[21] = 6;
	indices[22] = 3;
	indices[23] = 7;
}

bool Bound3::operator==(const Bound3& other) const {
	return (std::memcmp(this, &other, sizeof(Bound3)) == 0);
}

bool Bound3::operator!=(const Bound3& other) const {
	return (std::memcmp(this, &other, sizeof(Bound3)) != 0);
}

Bound2 enclose(const Bound2& a, const Bound2& b) {
	Vec2 min = minVec(a.getCornerMin(), b.getCornerMin());
	Vec2 max = maxVec(a.getCornerMax(), b.getCornerMax());
	return Bound2(min.x, min.y, max.x, max.y);
}

Bound3 enclose(const Bound3& a, const Bound3& b) {
	Vec3 min = minVec(a.getCornerMin(), b.getCornerMin());
	Vec3 max = maxVec(a.getCornerMax(), b.getCornerMax());
	return Bound3(min.x, min.y, min.z, max.x, max.y, max.z);
}

Vec2 randomPoint(const Bound2& a) {
	return Vec2(a.getCenter() + entrywiseProduct(Vec2(rand(-1.0f, +1.0f), rand(-1.0f, +1.0f)), a.getExtent()));
}

Vec3 randomPoint(const Bound3& a) {
	return Vec3(a.getCenter() + entrywiseProduct(Vec3(rand(-1.0f, +1.0f), rand(-1.0f, +1.0f), rand(-1.0f, +1.0f)), a.getExtent()));
}
