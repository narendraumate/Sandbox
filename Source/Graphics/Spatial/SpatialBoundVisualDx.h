//
//  SpatialBoundVisualDx.h
//
//
//  Created by Narendra Umate on 9/4/16.
//
//

#pragma once

#include "BufferDx.h"
#include "SpatialBoundVisual.h"

class SpatialBoundVisualDx3 : public SpatialBoundVisual3 {
public:
	SpatialBoundVisualDx3(const size_t& hash, const Mesh& mesh, const Material& material);
	virtual ~SpatialBoundVisualDx3();
};
