//
//  SpatialBoundVisualGl.cpp
//
//
//  Created by Narendra Umate on 9/4/16.
//
//

#include "SpatialBoundVisualGl.h"

SpatialBoundVisualGl3::SpatialBoundVisualGl3(const size_t& hash, const Mesh& mesh, const Material& material)
: SpatialBoundVisual3(hash, mesh, material) {
	m_visualMaterial = new VisualMaterialGl(material);
	m_vertexBuffer = new VertexBufferGl(mesh.vertices);
	if (mesh.points.count(0) > 0) {
		m_indexBuffers[0] = new IndexBufferGl(IndexBuffer::Point, mesh.points.at(0).indices);
	}
	if (mesh.lines.count(0) > 0) {
		m_indexBuffers[1] = new IndexBufferGl(IndexBuffer::Line, mesh.lines.at(0).indices);
	}
	if (mesh.triangles.count(0) > 0) {
		m_indexBuffers[2] = new IndexBufferGl(IndexBuffer::Triangle, mesh.triangles.at(0).indices);
	}
}

SpatialBoundVisualGl3::~SpatialBoundVisualGl3() {
	if (m_visualMaterial) {
		delete m_visualMaterial;
		m_visualMaterial = nullptr;
	}
	if (m_vertexBuffer) {
		delete m_vertexBuffer;
		m_vertexBuffer = nullptr;
	}
	if (m_indexBuffers[0]) {
		delete m_indexBuffers[0];
		m_indexBuffers[0] = nullptr;
	}
	if (m_indexBuffers[1]) {
		delete m_indexBuffers[1];
		m_indexBuffers[1] = nullptr;
	}
	if (m_indexBuffers[2]) {
		delete m_indexBuffers[2];
		m_indexBuffers[2] = nullptr;
	}
	if (m_indexBuffers[3]) {
		delete m_indexBuffers[3];
		m_indexBuffers[3] = nullptr;
	}
	if (m_indexBuffers[4]) {
		delete m_indexBuffers[4];
		m_indexBuffers[4] = nullptr;
	}
}
