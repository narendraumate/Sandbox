//
//  SpatialBoundVisualGl.h
//
//
//  Created by Narendra Umate on 9/4/16.
//
//

#pragma once

#include "BufferGl.h"
#include "SpatialBoundVisual.h"

class SpatialBoundVisualGl3 : public SpatialBoundVisual3 {
public:
	SpatialBoundVisualGl3(const size_t& hash, const Mesh& mesh, const Material& material);
	virtual ~SpatialBoundVisualGl3();
};
