//
//  VisualScreenSpaceGl.cpp
//
//
//  Created by Narendra Umate on 10/16/16.
//
//

#include "VisualScreenSpaceGl.h"

VisualScreenSpaceGl::VisualScreenSpaceGl(const Effect& effect)
: VisualScreenSpace(effect) {
	m_visualEffect = new VisualEffectGl(effect);
	std::vector<P2T2> vertices;
	vertices.push_back(P2T2(Vec2(-1.0f, -1.0f), Vec2(0.0f, 0.0f)));
	vertices.push_back(P2T2(Vec2(+1.0f, -1.0f), Vec2(1.0f, 0.0f)));
	vertices.push_back(P2T2(Vec2(-1.0f, +1.0f), Vec2(0.0f, 1.0f)));
	vertices.push_back(P2T2(Vec2(+1.0f, +1.0f), Vec2(1.0f, 1.0f)));
	m_vertexBuffer = new VertexBufferGl(vertices);
	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	m_indexBuffers[0] = new IndexBufferGl(IndexBuffer::Triangle, indices);
}

VisualScreenSpaceGl::~VisualScreenSpaceGl() {
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
