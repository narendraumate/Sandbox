//
//  SpatialBoundVisualVk.h
//
//
//  Created by Narendra Umate on 9/4/16.
//
//

#pragma once

#include "BufferVk.h"
#include "SpatialBoundVisual.h"

class SpatialBoundVisualVk3 : public SpatialBoundVisual3 {
public:
	SpatialBoundVisualVk3(const size_t& hash, const Mesh& mesh, const Material& material);
	virtual ~SpatialBoundVisualVk3();
};
