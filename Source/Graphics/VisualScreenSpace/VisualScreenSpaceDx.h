//
//  VisualScreenSpaceDx.h
//
//
//  Created by Narendra Umate on 10/16/16.
//
//

#pragma once

#include "BufferDx.h"
#include "VisualScreenSpace.h"

class VisualScreenSpaceDx : public VisualScreenSpace {
public:
	VisualScreenSpaceDx(const Effect& effect);
	virtual ~VisualScreenSpaceDx();
};
