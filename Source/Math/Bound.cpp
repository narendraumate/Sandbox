//
//  Bound.cpp
//
//
//  Created by Narendra Umate on 8/24/15.
//
//

#include "Bound.h"

// Bound3

Bound3::Bound3() {
	Vec3 m_center = Vec3::zero;
	Vec3 m_extent = Vec3::zero;
	Vec3 m_cornerMin = Vec3::zero;
	Vec3 m_cornerMax = Vec3::zero;

	// Calculate planes
	m_planes[r] = Vec4(1.0f, 0.0f, 0.0f, -m_cornerMax.x);
	m_planes[l] = Vec4(-1.0f, 0.0f, 0.0f, m_cornerMin.x);
	m_planes[t] = Vec4(0.0f, 1.0f, 0.0f, -m_cornerMax.y);
	m_planes[b] = Vec4(0.0f, -1.0f, 0.0f, m_cornerMin.y);
	m_planes[n] = Vec4(0.0f, 0.0f, 1.0f, -m_cornerMax.z);
	m_planes[f] = Vec4(0.0f, 0.0f, -1.0f, m_cornerMin.z);

	// Calculate points
	m_points[lbf] = Vec3(-1, -1, -1) * m_extent + m_center;
	m_points[lbn] = Vec3(-1, -1, +1) * m_extent + m_center;
	m_points[ltf] = Vec3(-1, +1, -1) * m_extent + m_center;
	m_points[ltn] = Vec3(-1, +1, +1) * m_extent + m_center;
	m_points[rbf] = Vec3(+1, -1, -1) * m_extent + m_center;
	m_points[rbn] = Vec3(+1, -1, +1) * m_extent + m_center;
	m_points[rtf] = Vec3(+1, +1, -1) * m_extent + m_center;
	m_points[rtn] = Vec3(+1, +1, +1) * m_extent + m_center;
}

Bound3::Bound3(const Vec3& center, const Vec3& extent) {
	Vec3 m_center = center;
	Vec3 m_extent = extent;

	Vec3 m_cornerMin = center - extent;
	Vec3 m_cornerMax = center + extent;

	// Calculate planes
	m_planes[r] = Vec4(1.0f, 0.0f, 0.0f, -m_cornerMax.x);
	m_planes[l] = Vec4(-1.0f, 0.0f, 0.0f, m_cornerMin.x);
	m_planes[t] = Vec4(0.0f, 1.0f, 0.0f, -m_cornerMax.y);
	m_planes[b] = Vec4(0.0f, -1.0f, 0.0f, m_cornerMin.y);
	m_planes[n] = Vec4(0.0f, 0.0f, 1.0f, -m_cornerMax.z);
	m_planes[f] = Vec4(0.0f, 0.0f, -1.0f, m_cornerMin.z);

	// Calculate points
	m_points[lbf] = Vec3(-1, -1, -1) * m_extent + m_center;
	m_points[lbn] = Vec3(-1, -1, +1) * m_extent + m_center;
	m_points[ltf] = Vec3(-1, +1, -1) * m_extent + m_center;
	m_points[ltn] = Vec3(-1, +1, +1) * m_extent + m_center;
	m_points[rbf] = Vec3(+1, -1, -1) * m_extent + m_center;
	m_points[rbn] = Vec3(+1, -1, +1) * m_extent + m_center;
	m_points[rtf] = Vec3(+1, +1, -1) * m_extent + m_center;
	m_points[rtn] = Vec3(+1, +1, +1) * m_extent + m_center;
}

Bound3::Bound3(const std::vector<P4N4T4B4T2P2>& vertices) {
	Vec3 m_cornerMin = Vec3::max;
	Vec3 m_cornerMax = Vec3::min;
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

	Vec3 m_center = m_cornerMin * 0.5f + m_cornerMax * 0.5f;
	Vec3 m_extent = m_cornerMax * 0.5f - m_cornerMin * 0.5f;

	// Calculate planes
	m_planes[r] = Vec4(1.0f, 0.0f, 0.0f, -m_cornerMax.x);
	m_planes[l] = Vec4(-1.0f, 0.0f, 0.0f, m_cornerMin.x);
	m_planes[t] = Vec4(0.0f, 1.0f, 0.0f, -m_cornerMax.y);
	m_planes[b] = Vec4(0.0f, -1.0f, 0.0f, m_cornerMin.y);
	m_planes[n] = Vec4(0.0f, 0.0f, 1.0f, -m_cornerMax.z);
	m_planes[f] = Vec4(0.0f, 0.0f, -1.0f, m_cornerMin.z);

	// Calculate points
	m_points[lbf] = Vec3(-1, -1, -1) * m_extent + m_center;
	m_points[lbn] = Vec3(-1, -1, +1) * m_extent + m_center;
	m_points[ltf] = Vec3(-1, +1, -1) * m_extent + m_center;
	m_points[ltn] = Vec3(-1, +1, +1) * m_extent + m_center;
	m_points[rbf] = Vec3(+1, -1, -1) * m_extent + m_center;
	m_points[rbn] = Vec3(+1, -1, +1) * m_extent + m_center;
	m_points[rtf] = Vec3(+1, +1, -1) * m_extent + m_center;
	m_points[rtn] = Vec3(+1, +1, +1) * m_extent + m_center;
}

Vec3 Bound3::getCenter() const {
	Vec3 center = Vec3::zero;
	for (int i = 0; i < 8; i++) {
		center += m_points[i] * 0.125f;
	}
	return center;
}

Vec3 Bound3::getExtent() const {
	Vec3 min = Vec3::max;
	Vec3 max = Vec3::min;
	for (unsigned int i = 0; i < 8; i++) {
		min = minVec(min, m_points[i]);
		max = maxVec(max, m_points[i]);
	}
	return max * 0.5f - min * 0.5f;
}

Vec3 Bound3::getCornerMin() const {
	Vec3 min = Vec3::max;
	for (unsigned int i = 0; i < 8; ++i) {
		min = minVec(min, m_points[i]);
	}
	return min;
}

Vec3 Bound3::getCornerMax() const {
	Vec3 max = Vec3::min;
	for (unsigned int i = 0; i < 8; ++i) {
		max = maxVec(max, m_points[i]);
	}
	return max;
}

bool Bound3::contains(const Vec3& point) const {
	Vec3 m_cornerMin = getCornerMin();
	Vec3 m_cornerMax = getCornerMax();
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
	return (contains(bound.getCornerMin()) && contains(bound.getCornerMax()));
}

Bound3 Bound3::transform(const Mat4& m) const {
	Vec3 oldCenter = getCenter();
	Vec3 oldExtent = getExtent();
	Vec3 newCenter = (m * positionVector(oldCenter)).project();
	Vec3 newExtent = (m * directionVector(oldExtent)).project();
	return Bound3(newCenter, newExtent);
}

Bound3 Bound3::transform(const Transform3& t) const {
	return transform(t.getMatrix());
}

P4N4T4B4T2P2 Bound3::getCornerVertex(const short& octant) const {
	switch (octant % 8) {
		case lbf:
			return P4N4T4B4T2P2(m_points[lbf], Vec3(-1, -1, -1), Vec2(0, 0));
		case lbn:
			return P4N4T4B4T2P2(m_points[lbn], Vec3(-1, -1, +1), Vec2(0, 1));
		case ltf:
			return P4N4T4B4T2P2(m_points[ltf], Vec3(-1, +1, -1), Vec2(1, 0));
		case ltn:
			return P4N4T4B4T2P2(m_points[ltn], Vec3(-1, +1, +1), Vec2(1, 1));
		case rbf:
			return P4N4T4B4T2P2(m_points[rbf], Vec3(+1, -1, -1), Vec2(0, 0));
		case rbn:
			return P4N4T4B4T2P2(m_points[rbn], Vec3(+1, -1, +1), Vec2(0, 1));
		case rtf:
			return P4N4T4B4T2P2(m_points[rtf], Vec3(+1, +1, -1), Vec2(1, 0));
		case rtn:
			return P4N4T4B4T2P2(m_points[rtn], Vec3(+1, +1, +1), Vec2(1, 1));
		default:
			return P4N4T4B4T2P2(Vec3(), Vec3(), Vec2());
	}
}

void Bound3::getCornerVertices(std::vector<P4N4T4B4T2P2>& vertices) const {
	vertices.resize(8);
	vertices[lbf] = P4N4T4B4T2P2(getCornerVertex(lbf));
	vertices[lbn] = P4N4T4B4T2P2(getCornerVertex(lbn));
	vertices[ltf] = P4N4T4B4T2P2(getCornerVertex(ltf));
	vertices[ltn] = P4N4T4B4T2P2(getCornerVertex(ltn));
	vertices[rbf] = P4N4T4B4T2P2(getCornerVertex(rbf));
	vertices[rbn] = P4N4T4B4T2P2(getCornerVertex(rbn));
	vertices[rtf] = P4N4T4B4T2P2(getCornerVertex(rtf));
	vertices[rtn] = P4N4T4B4T2P2(getCornerVertex(rtn));
}

void Bound3::getCornerIndices(std::vector<unsigned int>& indices) const {
	indices.resize(24);
	// Edge between lbn and lbf
	indices[0] = lbn;
	indices[1] = lbf;
	// Edge between lbf and ltf
	indices[2] = lbf;
	indices[3] = ltf;
	// Edge between ltf and ltn
	indices[4] = ltf;
	indices[5] = ltn;
	// Edge between ltn and lbn
	indices[6] = ltn;
	indices[7] = lbn;
	// Edge between rbn and rbf
	indices[8] = rbn;
	indices[9] = rbf;
	// Edge between rbf and rtf
	indices[10] = rbf;
	indices[11] = rtf;
	// Edge between rtf and rtn
	indices[12] = rtf;
	indices[13] = rtn;
	// Edge between rtn and rbn
	indices[14] = rtn;
	indices[15] = rbn;
	// Edge between lbn and rbn
	indices[16] = lbn;
	indices[17] = rbn;
	// Edge between lbf and rbf
	indices[18] = lbf;
	indices[19] = rbf;
	// Edge between ltn and rtn
	indices[20] = ltn;
	indices[21] = rtn;
	// Edge between ltf and rtf
	indices[22] = ltf;
	indices[23] = rtf;
}

bool Bound3::operator==(const Bound3& other) const {
	return (std::memcmp(this, &other, sizeof(Bound3)) == 0);
}

bool Bound3::operator!=(const Bound3& other) const {
	return (std::memcmp(this, &other, sizeof(Bound3)) != 0);
}

Bound3 enclose(const Bound3& a, const Bound3& b) {
	Vec3 min = minVec(a.getCornerMin(), b.getCornerMin());
	Vec3 max = maxVec(a.getCornerMax(), b.getCornerMax());
	Vec3 center = (min + max) * 0.5f;
	Vec3 extent = (max - min) * 0.5f;
	return Bound3(center, extent);
}

Vec3 randomPoint(const Bound3& a) {
	return Vec3(a.getCenter() + entrywiseProduct(Vec3(rand(-1.0f, +1.0f), rand(-1.0f, +1.0f), rand(-1.0f, +1.0f)), a.getExtent()));
}
