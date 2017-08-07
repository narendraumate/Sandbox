//
//  Frustum.cpp
//
//
//  Created by Narendra Umate on 4/13/13.
//
//

#include "Frustum.h"

Frustum2::Frustum2() {
}

Frustum2::~Frustum2() {
}

void Frustum2::setMatrix(const Mat3& matrix) {
	// TODO
}

Frustum3::Frustum3() {
}

Frustum3::~Frustum3() {
}

void Frustum3::setMatrix(const Mat4& matrixO, const bool& reverseZ) {
	// this is the view projection matrix so to flip projection axis we apppend the z flip matrix at the end
	// note that to go from forward projections to reverse and back the matrix multiplication needed is same
	Mat4 reverseM = Mat4::identity;
	reverseM.d[2][2] = -1.0f;
	reverseM.d[3][2] = +1.0f;
	Mat4 matrix = reverseZ ? reverseM * matrixO : matrixO;

	// right
	m_planes[r].x = matrix.r[3] - matrix.r[0];
	m_planes[r].y = matrix.r[7] - matrix.r[4];
	m_planes[r].z = matrix.r[11] - matrix.r[8];
	m_planes[r].w = matrix.r[15] - matrix.r[12];

	// left
	m_planes[l].x = matrix.r[3] + matrix.r[0];
	m_planes[l].y = matrix.r[7] + matrix.r[4];
	m_planes[l].z = matrix.r[11] + matrix.r[8];
	m_planes[l].w = matrix.r[15] + matrix.r[12];

	// top
	m_planes[t].x = matrix.r[3] - matrix.r[1];
	m_planes[t].y = matrix.r[7] - matrix.r[5];
	m_planes[t].z = matrix.r[11] - matrix.r[9];
	m_planes[t].w = matrix.r[15] - matrix.r[13];

	// bottom
	m_planes[b].x = matrix.r[3] + matrix.r[1];
	m_planes[b].y = matrix.r[7] + matrix.r[5];
	m_planes[b].z = matrix.r[11] + matrix.r[9];
	m_planes[b].w = matrix.r[15] + matrix.r[13];

	// near
	m_planes[n].x = matrix.r[3] + matrix.r[2];
	m_planes[n].y = matrix.r[7] + matrix.r[6];
	m_planes[n].z = matrix.r[11] + matrix.r[10];
	m_planes[n].w = matrix.r[15] + matrix.r[14];

	// far
	m_planes[f].x = matrix.r[3] - matrix.r[2];
	m_planes[f].y = matrix.r[7] - matrix.r[6];
	m_planes[f].z = matrix.r[11] - matrix.r[10];
	m_planes[f].w = matrix.r[15] - matrix.r[14];

	// planes
	m_planes[r] /= m_planes[r].project().norm();
	m_planes[l] /= m_planes[l].project().norm();
	m_planes[t] /= m_planes[t].project().norm();
	m_planes[b] /= m_planes[b].project().norm();
	m_planes[n] /= m_planes[n].project().norm();
	m_planes[f] /= m_planes[f].project().norm();

	// points
	m_points[lbn] = pointFromPlanes(m_planes[l], m_planes[b], m_planes[n]); // left,  bottom, near
	m_points[rbn] = pointFromPlanes(m_planes[r], m_planes[b], m_planes[n]); // right, bottom, near
	m_points[rtn] = pointFromPlanes(m_planes[r], m_planes[t], m_planes[n]); // right, top,    near
	m_points[ltn] = pointFromPlanes(m_planes[l], m_planes[t], m_planes[n]); // left,  top,    near
	m_points[lbf] = pointFromPlanes(m_planes[l], m_planes[b], m_planes[f]); // left,  bottom, far
	m_points[rbf] = pointFromPlanes(m_planes[r], m_planes[b], m_planes[f]); // right, bottom, far
	m_points[rtf] = pointFromPlanes(m_planes[r], m_planes[t], m_planes[f]); // right, top,    far
	m_points[ltf] = pointFromPlanes(m_planes[l], m_planes[t], m_planes[f]); // left,  top,    far
}
