//
//  Projection.cpp
//
//
//  Created by Narendra Umate on 11/4/12.
//
//

#if defined(__linux__) || defined(_WIN32)
#include <assert.h>
#endif

#include "Projection.h"

Mat4 makeProjection(Mat4 m_projectionNdc, ProjectionType m_projectionType, ProjectionRange m_projectionRange, ProjectionParameters m_projectionParameters, AxisDirectionConvention m_axisDirectionConvention) {
	assert(m_axisDirectionConvention == RUB);

	float rpl = (m_projectionParameters.r + m_projectionParameters.l);
	float tpb = (m_projectionParameters.t + m_projectionParameters.b);
	float fpn = (m_projectionParameters.f + m_projectionParameters.n);
	float rml = (m_projectionParameters.r - m_projectionParameters.l);
	float tmb = (m_projectionParameters.t - m_projectionParameters.b);
	float fmn = (m_projectionParameters.f - m_projectionParameters.n);

	// left hand and right hand affect third column of projection matrix
	float handL = -1.0f;
	float handR = +1.0f;
	float handS = false ? handL : handR; // OpenGL RUB and Metal RUB

	if (m_projectionType == Orthographic) {
		Mat4 m_orthographicProjectionMatrix;
		// update orthographic projection matrix
		float c = 0.0f;
		float d = 0.0f;

		switch (m_projectionRange) {
			default:
			case ForwardFiniteZ: //works but frustum culling broken
				c = -2.0f / fmn;
				d = -1.0f * fpn / fmn;
				break;
			case ForwardInfiniteZ: //works
				c = -0.0f;
				d = -1.0f;
				break;
			case ReverseFiniteZ: //works but frustum culling broken
				c = +2.0f / fmn;
				d = +2.0f * m_projectionParameters.f / fmn;
				break;
			case ReverseInfiniteZ: //works
				c = +0.0f;
				d = +2.0f;
				break;
		}

		m_orthographicProjectionMatrix.d[0][0] = 2.0f / rml;
		m_orthographicProjectionMatrix.d[0][1] = 0.0f;
		m_orthographicProjectionMatrix.d[0][2] = 0.0f;
		m_orthographicProjectionMatrix.d[0][3] = 0.0f;

		m_orthographicProjectionMatrix.d[1][0] = 0.0f;
		m_orthographicProjectionMatrix.d[1][1] = 2.0f / tmb;
		m_orthographicProjectionMatrix.d[1][2] = 0.0f;
		m_orthographicProjectionMatrix.d[1][3] = 0.0f;

		m_orthographicProjectionMatrix.d[2][0] = handS * 0.0f;
		m_orthographicProjectionMatrix.d[2][1] = handS * 0.0f;
		m_orthographicProjectionMatrix.d[2][2] = handS * c;
		m_orthographicProjectionMatrix.d[2][3] = handS * 0.0f;

		m_orthographicProjectionMatrix.d[3][0] = -rpl / rml;
		m_orthographicProjectionMatrix.d[3][1] = -tpb / tmb;
		m_orthographicProjectionMatrix.d[3][2] = d;
		m_orthographicProjectionMatrix.d[3][3] = 1.0f;

		m_orthographicProjectionMatrix = m_projectionNdc * m_orthographicProjectionMatrix;
		return m_orthographicProjectionMatrix;
	} else {
		Mat4 m_perspectiveProjectionMatrix;
		// update perspective projection matrix
		float c = 0.0f;
		float d = 0.0f;

		switch (m_projectionRange) {
			default:
			case ForwardFiniteZ: //works but frustum culling broken
				c = -1.0f * fpn / fmn;
				d = -2.0f * m_projectionParameters.f * m_projectionParameters.n / fmn;
				break;
			case ForwardInfiniteZ: //works
				c = -1.0f;
				d = -2.0f * m_projectionParameters.n;
				break;
			case ReverseFiniteZ: //works but frustum culling broken
				c = +2.0f * m_projectionParameters.n / fmn;
				d = +2.0f * m_projectionParameters.f * m_projectionParameters.n / fmn;
				break;
			case ReverseInfiniteZ: //works
				c = +0.0f;
				d = +2.0f * m_projectionParameters.n;
				break;
		}

		m_perspectiveProjectionMatrix.d[0][0] = 2.0f * m_projectionParameters.n / rml;
		m_perspectiveProjectionMatrix.d[0][1] = 0.0f;
		m_perspectiveProjectionMatrix.d[0][2] = 0.0f;
		m_perspectiveProjectionMatrix.d[0][3] = 0.0f;

		m_perspectiveProjectionMatrix.d[1][0] = 0.0f;
		m_perspectiveProjectionMatrix.d[1][1] = 2.0f * m_projectionParameters.n / tmb;
		m_perspectiveProjectionMatrix.d[1][2] = 0.0f;
		m_perspectiveProjectionMatrix.d[1][3] = 0.0f;

		m_perspectiveProjectionMatrix.d[2][0] = handS * rpl / rml;
		m_perspectiveProjectionMatrix.d[2][1] = handS * tpb / tmb;
		m_perspectiveProjectionMatrix.d[2][2] = handS * c;
		m_perspectiveProjectionMatrix.d[2][3] = handS * -1.0f;

		m_perspectiveProjectionMatrix.d[3][0] = 0.0f;
		m_perspectiveProjectionMatrix.d[3][1] = 0.0f;
		m_perspectiveProjectionMatrix.d[3][2] = d;
		m_perspectiveProjectionMatrix.d[3][3] = 0.0f;

		m_perspectiveProjectionMatrix = m_projectionNdc * m_perspectiveProjectionMatrix;
		return m_perspectiveProjectionMatrix;
	}
}

Mat4 warpProjection(Mat4 m_projectionNdc, ProjectionType m_projectionType, ProjectionRange m_projectionRange, ProjectionParameters m_projectionParameters, AxisDirectionConvention m_axisDirectionConvention, Mat4 oldView, Mat4 newView) {
	assert(m_axisDirectionConvention == RUB);

	ProjectionParameters l_projectionParameters(0, 0, 0, 0, 0, 0);

	Mat4 oldCamera = oldView.inverse();
	Mat4 newCamera = Mat4::translate(Transform3(newView.inverse()).getTranslate()) * Mat4::rotate(Transform3(oldCamera).getRotate());
	Mat4 transform = (newCamera.inverse() * oldCamera);

	Vec4 lt = transform * Vec4(m_projectionParameters.l * m_projectionParameters.n, m_projectionParameters.t * m_projectionParameters.n, -m_projectionParameters.n, 1.0f);
	Vec4 rt = transform * Vec4(m_projectionParameters.r * m_projectionParameters.n, m_projectionParameters.t * m_projectionParameters.n, -m_projectionParameters.n, 1.0f);
	Vec4 lb = transform * Vec4(m_projectionParameters.l * m_projectionParameters.n, m_projectionParameters.b * m_projectionParameters.n, -m_projectionParameters.n, 1.0f);
	Vec4 rb = transform * Vec4(m_projectionParameters.r * m_projectionParameters.n, m_projectionParameters.b * m_projectionParameters.n, -m_projectionParameters.n, 1.0f);

	l_projectionParameters.n = -0.25f * (lt.z + rt.z + lb.z + rb.z);
	l_projectionParameters.f = m_projectionParameters.f + (l_projectionParameters.n - m_projectionParameters.n);
	l_projectionParameters.l = +0.5f * (lt.x + lb.x) / l_projectionParameters.n;
	l_projectionParameters.r = +0.5f * (rt.x + rb.x) / l_projectionParameters.n;
	l_projectionParameters.b = +0.5f * (lb.y + rb.y) / l_projectionParameters.n;
	l_projectionParameters.t = +0.5f * (lt.y + rt.y) / l_projectionParameters.n;

	return makeProjection(m_projectionNdc, m_projectionType, m_projectionRange, l_projectionParameters, m_axisDirectionConvention);
}
