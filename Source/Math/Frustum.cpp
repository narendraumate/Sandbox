//
//  Frustum.cpp
//
//
//  Created by Narendra Umate on 4/13/13.
//
//

#include "Frustum.h"

Frustum3::Frustum3() {
}

Frustum3::~Frustum3() {
}

void Frustum3::setMatrix(const ProjectionDimension& ProjectionDimension, const Mat4& matrixO, const bool& reverseZ) {
	Mat4 invndc = ProjectionDimension == X2Y2Z2 ? Mat4::identity : Mat4::translate(Vec3(0.0f, 0.0f, -1.0f)) * Mat4::scale(Vec3(1.0f, 1.0f, 2.0f));

	Mat4 matrix;

	if (reverseZ) {
		// this is the view projection matrix so to flip projection axis we apppend the z flip matrix at the end
		// note that to go from forward projections to reverse and back the matrix multiplication needed is same
		Mat4 reverseM = Mat4::identity;
		reverseM.d[2][2] = -1.0f;
		reverseM.d[3][2] = +1.0f;
		matrix = reverseM * invndc * matrixO;
	} else {
		matrix = invndc * matrixO;
	}

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

	std::cout << "Old" << std::endl;

	for (int i = 0; i < 6; ++i) {
		std::cout << "Plane: " << i << " " << m_planes[i].x << " " << m_planes[i].y << " " << m_planes[i].z << " " << m_planes[i].w << std::endl;
	}

	for (int i = 0; i < 8; ++i) {
		std::cout << "Point: " << i << " " << m_points[i].x << " " << m_points[i].y << " " << m_points[i].z << std::endl;
	}
}

void Frustum3::set(const Mat4& cameraTransformMatrix, const ProjectionParameter& projection, const bool& infinite) {
	const float* cameraTransform = cameraTransformMatrix.r;

	float cameraPosition[3] = { cameraTransform[12], cameraTransform[13], cameraTransform[14] };
	float right[3] = { cameraTransform[0], cameraTransform[1], cameraTransform[2] };
	float up[3] = { cameraTransform[4], cameraTransform[5], cameraTransform[6] };
	float forward[3] = { -cameraTransform[8], -cameraTransform[9], -cameraTransform[10] };

	// Calculate the four corners of the near plane
	float halfWidthNear = projection.n() * (projection.r() - projection.l()) * 0.5f;
	float halfHeightNear = projection.n() * (projection.t() - projection.b()) * 0.5f;

	float nearCenter[3] = {
		cameraPosition[0] + forward[0] * projection.n(),
		cameraPosition[1] + forward[1] * projection.n(),
		cameraPosition[2] + forward[2] * projection.n()
	};

	// Four corners of the near plane
	float nearTopLeft[3] = {
		nearCenter[0] + up[0] * halfHeightNear - right[0] * halfWidthNear,
		nearCenter[1] + up[1] * halfHeightNear - right[1] * halfWidthNear,
		nearCenter[2] + up[2] * halfHeightNear - right[2] * halfWidthNear
	};

	float nearTopRight[3] = {
		nearCenter[0] + up[0] * halfHeightNear + right[0] * halfWidthNear,
		nearCenter[1] + up[1] * halfHeightNear + right[1] * halfWidthNear,
		nearCenter[2] + up[2] * halfHeightNear + right[2] * halfWidthNear
	};

	float nearBottomLeft[3] = {
		nearCenter[0] - up[0] * halfHeightNear - right[0] * halfWidthNear,
		nearCenter[1] - up[1] * halfHeightNear - right[1] * halfWidthNear,
		nearCenter[2] - up[2] * halfHeightNear - right[2] * halfWidthNear
	};

	float nearBottomRight[3] = {
		nearCenter[0] - up[0] * halfHeightNear + right[0] * halfWidthNear,
		nearCenter[1] - up[1] * halfHeightNear + right[1] * halfWidthNear,
		nearCenter[2] - up[2] * halfHeightNear + right[2] * halfWidthNear
	};

	// Calculate the four corners of the far plane
	float halfWidthFar = projection.f() * (projection.r() - projection.l()) * 0.5f;
	float halfHeightFar = projection.f() * (projection.t() - projection.b()) * 0.5f;

	float farCenter[3] = {
		cameraPosition[0] + forward[0] * projection.f(),
		cameraPosition[1] + forward[1] * projection.f(),
		cameraPosition[2] + forward[2] * projection.f()
	};

	// Four corners of the far plane
	float farTopLeft[3] = {
		farCenter[0] + up[0] * halfHeightFar - right[0] * halfWidthFar,
		farCenter[1] + up[1] * halfHeightFar - right[1] * halfWidthFar,
		farCenter[2] + up[2] * halfHeightFar - right[2] * halfWidthFar
	};

	float farTopRight[3] = {
		farCenter[0] + up[0] * halfHeightFar + right[0] * halfWidthFar,
		farCenter[1] + up[1] * halfHeightFar + right[1] * halfWidthFar,
		farCenter[2] + up[2] * halfHeightFar + right[2] * halfWidthFar
	};

	float farBottomLeft[3] = {
		farCenter[0] - up[0] * halfHeightFar - right[0] * halfWidthFar,
		farCenter[1] - up[1] * halfHeightFar - right[1] * halfWidthFar,
		farCenter[2] - up[2] * halfHeightFar - right[2] * halfWidthFar
	};

	float farBottomRight[3] = {
		farCenter[0] - up[0] * halfHeightFar + right[0] * halfWidthFar,
		farCenter[1] - up[1] * halfHeightFar + right[1] * halfWidthFar,
		farCenter[2] - up[2] * halfHeightFar + right[2] * halfWidthFar
	};

	// points
	m_points[lbn] = nearBottomLeft;
	m_points[rbn] = nearBottomRight;
	m_points[rtn] = nearTopRight;
	m_points[ltn] = nearTopLeft;
	m_points[lbf] = farBottomLeft;
	m_points[rbf] = farBottomRight;
	m_points[rtf] = farTopRight;
	m_points[ltf] = farTopLeft;

	// planes
	m_planes[l] = planeFromPoints(m_points[lbn], m_points[lbf], m_points[ltn]);
	m_planes[r] = planeFromPoints(m_points[rbn], m_points[rtn], m_points[rbf]);
	m_planes[b] = planeFromPoints(m_points[lbn], m_points[rbn], m_points[lbf]);
	m_planes[t] = planeFromPoints(m_points[ltf], m_points[rtf], m_points[ltn]);
	m_planes[f] = planeFromPoints(m_points[lbf], m_points[rbf], m_points[rtf]);
	m_planes[n] = planeFromPoints(m_points[lbn], m_points[ltn], m_points[rtn]);

	std::cout << "New" << std::endl;

	for (int i = 0; i < 6; ++i) {
		std::cout << "Plane: " << i << " " << m_planes[i].x << " " << m_planes[i].y << " " << m_planes[i].z << " " << m_planes[i].w << std::endl;
	}

	for (int i = 0; i < 8; ++i) {
		std::cout << "Point: " << i << " " << m_points[i].x << " " << m_points[i].y << " " << m_points[i].z << std::endl;
	}
}
