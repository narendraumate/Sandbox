//
//  Coeff.cpp
//
//
//  Created by Narendra Umate on 10/5/12.
//
//

#include "Coeff.h"

Coeff3f::Coeff3f() {
	kC = 1.0f;
	kL = 2.0f;
	kQ = 1.0f;
}

Coeff3f::Coeff3f(const float& radius) {
	kC = 1.0f;
	kL = 2.0f / radius;
	kQ = 1.0f / radius / radius;
}

Coeff3f::Coeff3f(const float& c, const float& l, const float& q) {
	kC = c;
	kL = l;
	kQ = q;
}

Vec3 Coeff3f::vec3() const {
	return Vec3(kC, kL, kQ);
}
