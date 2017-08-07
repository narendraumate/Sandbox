//
//  GlVisualScreenSpace.h
//
//
//  Created by Narendra Umate on 10/16/16.
//
//

#pragma once

#include "BufferGl.h"
#include "VisualScreenSpace.h"

class GlVisualScreenSpace : public VisualScreenSpace {
public:
	GlVisualScreenSpace(const Effect& effect);
	virtual ~GlVisualScreenSpace();
};
