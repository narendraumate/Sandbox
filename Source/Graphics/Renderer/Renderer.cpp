//
//  Renderer.cpp
//
//
//  Created by Narendra Umate on 9/7/13.
//
//

#include "Renderer.h"

Renderer::Renderer(const int& width, const int& height)
: m_width(width)
, m_height(height)
, m_clearColor(Color4f(0.5f, 0.5f, 0.5f, 1.0f))
, m_clearDepth(1.0f)
, m_clearStencil(1) {
}

Renderer::~Renderer() {
	m_width = 0;
	m_height = 0;
}

void Renderer::setNdcMatrix(const Mat4& matrix) {
	m_ndcMatrix = matrix;
}

Mat4 Renderer::getNdcMatrix() {
	return m_ndcMatrix;
}

void Renderer::setProjectionRange(const ProjectionRange& range) {
	m_projectionRange = range;
}

ProjectionRange Renderer::getProjectionRange() {
	return m_projectionRange;
}

void Renderer::setWidth(const int& width) {
	m_width = width;
}

int Renderer::getWidth() {
	return m_width;
}

void Renderer::setHeight(const int& height) {
	m_height = height;
}

int Renderer::getHeight() {
	return m_height;
}

void Renderer::draw(Bvh* bvh, VisualEffect* visualEffect, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, const Vec3& lightCoefficients, const Vec3& lightColor, const Vec3& lightPosition, const Vec3& eyePosition) {
	// Draw your self.
	draw(bvh->getBoundingBox(), visualEffect, visualMaterial, worldMatrix, viewMatrix, viewProjectionMatrix, lightCoefficients, lightColor, lightPosition, eyePosition);

	// Draw your children.
	int childCount = bvh->getChildCount();
	if (childCount) {
		for (int childIndex = 0; childIndex < childCount; ++childIndex) {
			draw(bvh->getChild(childIndex), visualEffect, visualMaterial, worldMatrix, viewMatrix, viewProjectionMatrix, lightCoefficients, lightColor, lightPosition, eyePosition);
		}
	}
}

void Renderer::draw(Octree* octree, VisualEffect* visualEffect, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, const Vec3& lightCoefficients, const Vec3& lightColor, const Vec3& lightPosition, const Vec3& eyePosition) {
	// Draw your self.
	draw(octree->getBoundingBox(), visualEffect, visualMaterial, worldMatrix, viewMatrix, viewProjectionMatrix, lightCoefficients, lightColor, lightPosition, eyePosition);

	// Draw your children.
	int childCount = octree->getChildCount();
	if (childCount) {
		for (int childIndex = 0; childIndex < childCount; ++childIndex) {
			draw(octree->getChild(childIndex), visualEffect, visualMaterial, worldMatrix, viewMatrix, viewProjectionMatrix, lightCoefficients, lightColor, lightPosition, eyePosition);
		}
	}
}
