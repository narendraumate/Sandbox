//
//  VisualScreenSpaceVk.cpp
//
//
//  Created by Narendra Umate on 10/16/16.
//
//

#include "VisualScreenSpaceVk.h"

VisualScreenSpaceVk::VisualScreenSpaceVk(const Effect& effect)
: VisualScreenSpace(effect) {
	m_visualEffect = new VisualEffectVk(effect);
	std::vector<P4N4T4B4T2P2> vertices;
	m_vertexBuffer = new VertexBufferVk(vertices);
	std::vector<unsigned int> indices;
	m_indexBuffers[0] = new IndexBufferVk(IndexBuffer::Triangle, indices);
}

VisualScreenSpaceVk::~VisualScreenSpaceVk() {
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
