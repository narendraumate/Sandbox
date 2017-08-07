//
//  VisualScreenSpaceMl.cpp
//
//
//  Created by Narendra Umate on 10/16/16.
//
//

#include "VisualScreenSpaceMl.h"

VisualScreenSpaceMl::VisualScreenSpaceMl(const Effect& effect)
: VisualScreenSpace(effect) {
	m_visualEffect = new VisualEffectMl(effect);
	std::vector<P4N4T4B4T2P2> vertices;
	m_vertexBuffer = new VertexBufferMl(vertices);
	std::vector<unsigned int> indices;
	m_indexBuffers[0] = new IndexBufferMl(IndexBuffer::Triangle, indices);
}

VisualScreenSpaceMl::~VisualScreenSpaceMl() {
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
