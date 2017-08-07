//
//  VkVisualScreenSpace.h
//
//
//  Created by Narendra Umate on 10/16/16.
//
//

#pragma once

#include "BufferVk.h"
#include "VisualScreenSpace.h"

class VkVisualScreenSpace : public VisualScreenSpace {
public:
	VkVisualScreenSpace(const Effect& effect);
	virtual ~VkVisualScreenSpace();
};
