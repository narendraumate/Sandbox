//
//  Coeff.h
//
//
//  Created by Narendra Umate on 10/5/12.
//
//

#pragma once

#include "Vector.h"

// Reference https://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/

class Coeff3f {
public:
	Coeff3f();
	Coeff3f(const float& radius);
	Coeff3f(const float& c, const float& l, const float& q);
	Vec3 vec3() const;

private:
	float kC = 1.0f;
	float kL = 2.0f;
	float kQ = 1.0f;
};
