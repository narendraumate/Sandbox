//
//  Projection.cpp
//
//
//  Created by Narendra Umate on 11/4/12.
//
//

#if defined(__APPLE__) || defined(__linux__) || defined(_WIN32)
#include <assert.h>
#endif

#include "Projection.h"

Projection::Projection(const ProjectionVariation& variation,
					   const ProjectionChirality& chirality,
					   const ProjectionDimension& dimension,
					   const ProjectionParameter& parameter) {
	ProjectionVariation m_variation = variation;
	ProjectionChirality m_chirality = chirality;
	ProjectionDimension m_dimension = dimension;
	ProjectionDirection m_direction = parameter.isForward() ? Forward : Reverse;
	ProjectionRestraint m_restraint = parameter.isFinite() ? Finite : Infinite;
	ProjectionParameter m_parameter = parameter;

	float rpl = (m_parameter.r() + m_parameter.l());
	float tpb = (m_parameter.t() + m_parameter.b());
	float fpn = (m_parameter.f() + m_parameter.n());
	float rml = (m_parameter.r() - m_parameter.l());
	float tmb = (m_parameter.t() - m_parameter.b());
	float fmn = (m_parameter.f() - m_parameter.n());

	float n = m_parameter.n();
	float f = m_parameter.f();

	// left hand and right hand affect third column of projection matrix
	float handL = -1.0f;
	float handR = +1.0f;
	float handS = m_chirality == Left ? handL : handR; // OpenGL RUB and Metal RUB

	float c = 0.0f;
	float d = 0.0f;

	if (m_variation == Perspective) {
		if (m_dimension == X2Y2Z2) {
			if (m_direction == Reverse) {
				if (m_restraint == Infinite) {
					c = 0.0f;
					d = +2.0f * n;
				} else if (m_restraint == Finite) {
					c = +2.0f * n / fmn;
					d = +2.0f * f * n / fmn;
				}
			} else if (m_direction == Forward) {
				if (m_restraint == Infinite) {
					c = -1.0f;
					d = -2.0f * n;
				} else if (m_restraint == Finite) {
					c = -1.0f * fpn / fmn;
					d = -2.0f * f * n / fmn;
				}
			}
		} else if (m_dimension == X2Y2Z1) {
			if (m_direction == Reverse) {
				/* It maybe looks like culling is broken for reverse projection */
				/* Until issues are resolved test intersect will always be true */
				if (m_restraint == Infinite) {
					c = 0.0f;
					d = +1.0f * n;
					printf("projection: reverse infinite: no render: maybe culling broken\n");
				} else if (m_restraint == Finite) {
					c = +1.0f * n / fmn;
					d = +1.0f * f * n / fmn;
					printf("projection: reverse finite: no render: maybe culling broken\n");
				}
			} else if (m_direction == Forward) {
				if (m_restraint == Infinite) {
					c = -1.0f;
					d = -1.0f * n;
					printf("projection: forward infinite: works fine\n");
				} else if (m_restraint == Finite) {
					c = -1.0f * f / fmn;
					d = -1.0f * f * n / fmn;
					printf("projection: forward finite: works fine\n");
				}
			}
		}
		m_matrix.r[0x0] = 2.0f / rml;
		m_matrix.r[0x1] = 0.0f;
		m_matrix.r[0x2] = 0.0f;
		m_matrix.r[0x3] = 0.0f;
		m_matrix.r[0x4] = 0.0f;
		m_matrix.r[0x5] = 2.0f / tmb;
		m_matrix.r[0x6] = 0.0f;
		m_matrix.r[0x7] = 0.0f;
		m_matrix.r[0x8] = handS * rpl / rml;
		m_matrix.r[0x9] = handS * tpb / tmb;
		m_matrix.r[0xA] = handS * c;
		m_matrix.r[0xB] = handS * -1.0f;
		m_matrix.r[0xC] = 0.0f;
		m_matrix.r[0xD] = 0.0f;
		m_matrix.r[0xE] = d;
		m_matrix.r[0xF] = 0.0f;
	} else if (m_variation == Orthographic) {
		if (m_dimension == X2Y2Z2) {
			if (m_direction == Reverse) {
				if (m_restraint == Infinite) {
					c = 0.00f; // ??
					d = +1.0f; // ??
				} else if (m_restraint == Finite) {
					c = +2.0f / fmn;
					d = +1.0f * fpn / fmn;
				}
			} else if (m_direction == Forward) {
				if (m_restraint == Infinite) {
					c = 0.00f; // ??
					d = -1.0f; // ??
				} else if (m_restraint == Finite) {
					c = -2.0f / fmn;
					d = -1.0f * fpn / fmn;
				}
			}
		} else if (m_dimension == X2Y2Z1) {
			if (m_direction == Reverse) {
				/* It maybe looks like culling is broken for reverse projection */
				/* Until issues are resolved test intersect will always be true */
				if (m_restraint == Infinite) {
					c = +M_EPSILON; // should be 0 but for solving artifacts epsilon from positive side.
					d = +1.0f;
				} else if (m_restraint == Finite) {
					c = +1.0f / fmn;
					d = +1.0f * f / fmn;
				}
			} else if (m_direction == Forward) {
				if (m_restraint == Infinite) {
					c = -M_EPSILON; // should be 0 but for solving artifacts epsilon from negative side.
					d = 0.00f;
				} else if (m_restraint == Finite) {
					c = -1.0f / fmn;
					d = -1.0f * n / fmn;
				}
			}
		}
		m_matrix.r[0x0] = 2.0f / rml;
		m_matrix.r[0x1] = 0.0f;
		m_matrix.r[0x2] = 0.0f;
		m_matrix.r[0x3] = 0.0f;
		m_matrix.r[0x4] = 0.0f;
		m_matrix.r[0x5] = 2.0f / tmb;
		m_matrix.r[0x6] = 0.0f;
		m_matrix.r[0x7] = 0.0f;
		m_matrix.r[0x8] = handS * 0.0f;
		m_matrix.r[0x9] = handS * 0.0f;
		m_matrix.r[0xA] = handS * c;
		m_matrix.r[0xB] = handS * 0.0f;
		m_matrix.r[0xC] = -rpl / rml;
		m_matrix.r[0xD] = -tpb / tmb;
		m_matrix.r[0xE] = d;
		m_matrix.r[0xF] = 1.0f;
	}
}

Projection::~Projection() {
}

Mat4 Projection::getMatrix() const {
	return m_matrix;
}
