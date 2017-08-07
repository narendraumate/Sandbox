//
//  SpatialBoundVisualMl.h
//
//
//  Created by Narendra Umate on 9/4/16.
//
//

#pragma once

#include "BufferMl.h"
#include "SpatialBoundVisual.h"

class SpatialBoundVisualMl3 : public SpatialBoundVisual3 {
public:
	SpatialBoundVisualMl3(const Mesh& mesh, const Material& material);
	virtual ~SpatialBoundVisualMl3();
};
