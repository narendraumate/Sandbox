//
//  SpatialBoundVisualDx.cpp
//
//
//  Created by Narendra Umate on 9/4/16.
//
//

#include "SpatialBoundVisualDx.h"

SpatialBoundVisualDx3::SpatialBoundVisualDx3(const Mesh& mesh, const Material& material)
: SpatialBoundVisual3(mesh, material) {
	m_visualMaterial = new VisualMaterialDx(material);
	m_vertexBuffer = new VertexBufferDx(mesh.vertices);
	if (mesh.primitives.at(0).points.indices.size()) {
		m_indexBuffers[0] = new IndexBufferDx(IndexBuffer::Point, mesh.primitives.at(0).points.indices);
	}
	if (mesh.primitives.at(0).edges.indices.size()) {
		m_indexBuffers[1] = new IndexBufferDx(IndexBuffer::Edge, mesh.primitives.at(0).edges.indices);
	}
	if (mesh.primitives.at(0).triangles.indices.size()) {
		m_indexBuffers[2] = new IndexBufferDx(IndexBuffer::Triangle, mesh.primitives.at(0).triangles.indices);
	}
	if (mesh.primitives.at(0).quadrilaterals.indices.size()) {
		m_indexBuffers[3] = new IndexBufferDx(IndexBuffer::Quadrilateral, mesh.primitives.at(0).quadrilaterals.indices);
	}
	if (mesh.primitives.at(0).faces.indices.size()) {
		m_indexBuffers[4] = new IndexBufferDx(IndexBuffer::Face, mesh.primitives.at(0).faces.indices, mesh.primitives.at(0).faces.counts);
	}

	std::vector<P4N4T4B4T2P2> boundingBoxVertices;
	m_modelBoundingBox.getCornerVertices(boundingBoxVertices);

	std::vector<unsigned int> boundingBoxIndices;
	m_modelBoundingBox.getCornerIndices(boundingBoxIndices);
}

SpatialBoundVisualDx3::~SpatialBoundVisualDx3() {
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
