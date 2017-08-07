//
//  SpatialLight.h
//
//
//  Created by Narendra Umate on 11/29/15.
//
//

#pragma once

#include "Coeff.h"
#include "Color.h"
#include "Spatial.h"

class SpatialLight : public Spatial3 {
public:
	SpatialLight(const Coeff3f& coefficients, const Color3f& color = Color3iWhite, const Transform3& transform = Transform3());
	virtual ~SpatialLight();

	Coeff3f getCoefficients();
	Color3f getColor();

private:
	Coeff3f m_coefficients;
	Color3f m_color;
};
