//
//  DxVisualScreenSpace.h
//
//
//  Created by Narendra Umate on 10/16/16.
//
//

#pragma once

#include "BufferDx.h"
#include "VisualScreenSpace.h"

class DxVisualScreenSpace : public VisualScreenSpace {
public:
	DxVisualScreenSpace(const Effect& effect);
	virtual ~DxVisualScreenSpace();
};
