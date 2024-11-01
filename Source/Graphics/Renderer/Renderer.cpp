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

void Renderer::setProjectionSize(const ProjectionSize& dimension) {
	m_projectionSize = dimension;
}

ProjectionSize Renderer::getProjectionSize() {
	return m_projectionSize;
}

void Renderer::setProjectionRestraint(const ProjectionDirectionRestraint& range) {
	m_projectionDirectionRestraint = range;
}

ProjectionDirectionRestraint Renderer::getProjectionDirectionRestraint() {
	return m_projectionDirectionRestraint;
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

void Renderer::draw(Bvh* bvh, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, VisualEffect* visualEffect, const std::vector<SpatialLight*>& spatialLights) {
	// Draw your self.
	draw(bvh->getBoundingBox(), visualMaterial, worldMatrix, viewMatrix, viewProjectionMatrix, visualEffect, spatialLights);

	// Draw your children.
	int childCount = bvh->getChildCount();
	if (childCount) {
		for (int childIndex = 0; childIndex < childCount; ++childIndex) {
			draw(bvh->getChild(childIndex), visualMaterial, worldMatrix, viewMatrix, viewProjectionMatrix, visualEffect, spatialLights);
		}
	}
}

void Renderer::draw(Octree* octree, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, VisualEffect* visualEffect, const std::vector<SpatialLight*>& spatialLights) {
	// Draw your self.
	draw(octree->getBoundingBox(), visualMaterial, worldMatrix, viewMatrix, viewProjectionMatrix, visualEffect, spatialLights);

	// Draw your children.
	int childCount = octree->getChildCount();
	if (childCount) {
		for (int childIndex = 0; childIndex < childCount; ++childIndex) {
			draw(octree->getChild(childIndex), visualMaterial, worldMatrix, viewMatrix, viewProjectionMatrix, visualEffect, spatialLights);
		}
	}
}
