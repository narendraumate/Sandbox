//
//  SpatialBoundVisual.cpp
//
//
//  Created by Narendra Umate on 8/28/15.
//
//

#include "SpatialBoundVisual.h"

SpatialBoundVisual3::SpatialBoundVisual3(const Mesh& mesh, const Material& material)
: m_visualMaterial(nullptr)
, m_vertexBuffer(nullptr) {
	m_indexBuffers[0] = nullptr;
	m_indexBuffers[1] = nullptr;
	m_indexBuffers[2] = nullptr;
	m_indexBuffers[3] = nullptr;
	m_indexBuffers[4] = nullptr;
	m_modelBoundingBox.create(mesh.vertices); // TODO Fix this
	updateWorldBoundingBox();
}

SpatialBoundVisual3::~SpatialBoundVisual3() {
}

void SpatialBoundVisual3::setTranslate(const Vec3& translate) {
	m_worldTransform.setTranslate(translate);
	updateWorldBoundingBox();
	updateWorldViewNormMatrix();
	updateWorldViewProjectionMatrix();
}

void SpatialBoundVisual3::setRotate(const Quat& rotate) {
	m_worldTransform.setRotate(rotate);
	updateWorldBoundingBox();
	updateWorldViewNormMatrix();
	updateWorldViewProjectionMatrix();
}

void SpatialBoundVisual3::setScale(const Vec3& scale) {
	m_worldTransform.setScale(scale);
	updateWorldBoundingBox();
	updateWorldViewNormMatrix();
	updateWorldViewProjectionMatrix();
}

void SpatialBoundVisual3::updateWorldViewNormMatrix() {
	m_worldViewNormMatrix = Mat3(m_viewMatrix * m_worldTransform.getMatrix()).inverse().transpose();
}

void SpatialBoundVisual3::updateWorldViewProjectionMatrix() {
	m_worldViewProjectionMatrix = m_viewProjectionMatrix * m_worldTransform.getMatrix();
}

void SpatialBoundVisual3::setViewProjectionMatrix(const Mat4& projectionMatrix, const Mat4& viewMatrix) {
	if (m_viewMatrix != viewMatrix) {
		m_viewMatrix = viewMatrix;
		updateWorldViewNormMatrix();
	}
	Mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
	if (m_viewProjectionMatrix != viewProjectionMatrix) {
		m_viewProjectionMatrix = viewProjectionMatrix;
		updateWorldViewProjectionMatrix();
	}
}
