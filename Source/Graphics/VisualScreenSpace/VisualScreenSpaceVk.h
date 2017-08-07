//
//  VisualScreenSpaceVk.h
//
//
//  Created by Narendra Umate on 10/16/16.
//
//

#pragma once

#include "BufferVk.h"
#include "VisualScreenSpace.h"

class VisualScreenSpaceVk : public VisualScreenSpace {
public:
	VisualScreenSpaceVk(const Effect& effect);
	virtual ~VisualScreenSpaceVk();
};
