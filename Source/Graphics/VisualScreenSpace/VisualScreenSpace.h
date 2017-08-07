//
//  VisualScreenSpace.h
//
//
//  Created by Narendra Umate on 7/15/17.
//
//

#pragma once

#include "Buffer.h"

class VisualScreenSpace {
public:
	VisualScreenSpace(const Effect& effect);
	virtual ~VisualScreenSpace();

	VisualEffect* getVisualEffect() {
		return m_visualEffect;
	}

	VertexBuffer* getVertexBuffer() {
		return m_vertexBuffer;
	}

	IndexBuffer* getIndexBuffer(const int& index) {
		return m_indexBuffers[index];
	}

protected:
	VisualEffect* m_visualEffect;
	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffers[1];
};
