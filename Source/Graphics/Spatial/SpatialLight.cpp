//
//  SpatialLight.cpp
//
//
//  Created by Narendra Umate on 11/29/15.
//
//

#include "SpatialLight.h"

SpatialLight::SpatialLight(const Coeff3f& coefficients, const Color3f& color, const Transform3& transform)
: Spatial3(transform)
, m_color(color)
, m_coefficients(coefficients) {
}

SpatialLight::~SpatialLight() {
}

Coeff3f SpatialLight::getCoefficients() {
	return m_coefficients;
}

Color3f SpatialLight::getColor() {
	return m_color;
}
