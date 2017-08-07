//
//  DxVisualScreenSpace.cpp
//
//
//  Created by Narendra Umate on 10/16/16.
//
//

#include "VisualScreenSpaceDx.h"

DxVisualScreenSpace::DxVisualScreenSpace(const Effect& effect)
: VisualScreenSpace(effect) {
	m_visualEffect = new VisualEffectDx(effect);
	std::vector<P4N4T4B4T2P2> vertices;
	m_vertexBuffer = new VertexBufferDx(vertices);
	std::vector<unsigned int> indices;
	m_indexBuffers[0] = new IndexBufferDx(IndexBuffer::Triangle, indices);
}

DxVisualScreenSpace::~DxVisualScreenSpace() {
	if (m_visualEffect) {
		delete m_visualEffect;
		m_visualEffect = nullptr;
	}
	if (m_vertexBuffer) {
		delete m_vertexBuffer;
		m_vertexBuffer = nullptr;
	}
	if (m_indexBuffers[0]) {
		delete m_indexBuffers[0];
		m_indexBuffers[0] = nullptr;
	}
}
