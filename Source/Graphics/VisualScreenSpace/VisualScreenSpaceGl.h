//
//  VisualScreenSpaceGl.h
//
//
//  Created by Narendra Umate on 10/16/16.
//
//

#pragma once

#include "BufferGl.h"
#include "VisualScreenSpace.h"

class VisualScreenSpaceGl : public VisualScreenSpace {
public:
	VisualScreenSpaceGl(const Effect& effect);
	virtual ~VisualScreenSpaceGl();
};
