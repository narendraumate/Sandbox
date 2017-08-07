//
//  MlVisualScreenSpace.h
//
//
//  Created by Narendra Umate on 10/16/16.
//
//

#pragma once

#include "BufferMl.h"
#include "VisualScreenSpace.h"

class MlVisualScreenSpace : public VisualScreenSpace {
public:
	MlVisualScreenSpace(const Effect& effect);
	virtual ~MlVisualScreenSpace();
};
