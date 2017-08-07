//
//  VisualScreenSpace.cpp
//
//
//  Created by Narendra Umate on 7/15/17.
//
//

#include "VisualScreenSpace.h"

VisualScreenSpace::VisualScreenSpace(const Effect& effect)
: m_visualEffect(nullptr)
, m_vertexBuffer(nullptr) {
	m_indexBuffers[0] = nullptr;
}

VisualScreenSpace::~VisualScreenSpace() {
}
