//
//  RendererDx.cpp
//
//
//  Created by Narendra Umate on 9/7/13.
//
//

#if defined(_WIN32)

#include "RendererDx.h"

RendererDx::RendererDx(const int& width, const int& height)
: Renderer(width, height) {
	m_ndcMatrix = Mat4::identity;
}

RendererDx::~RendererDx() {
}

void RendererDx::clear() {
}

void RendererDx::setClearColor(const Color4f& clearColor) {
	m_clearColor = clearColor;
}

Color4f RendererDx::getClearColor() const {
	return m_clearColor;
}

void RendererDx::setClearDepth(const float& clearDepth) {
	m_clearDepth = clearDepth;
}

float RendererDx::getClearDepth() const {
	return m_clearDepth;
}

void RendererDx::setClearStencil(const unsigned int& clearStencil) {
	m_clearStencil = clearStencil;
}

unsigned int RendererDx::getClearStencil() const {
	return m_clearStencil;
}

void RendererDx::setViewport(const int& xPos, const int& yPos, const int& w, const int& h) {
}

void RendererDx::getViewport(int& xPos, int& yPos, int& w, int& h) const {
}

void RendererDx::setSize(const int& width, const int& height) {
}

void RendererDx::getSize(int& width, int& height) {
}

void RendererDx::blit(const std::vector<Framebuffer*>& framebuffers, const int& cols, const int& rows, const int& width, const int& height, void* texture, void* drawable) {
}

void RendererDx::beginFrame() {
}

void RendererDx::endFrame() {
}

void RendererDx::bind(Framebuffer* framebuffer, const Framebuffer::Target& target) {
	framebuffer->bind(target);
}

void RendererDx::unbind(Framebuffer* framebuffer, const Framebuffer::Target& target) {
	framebuffer->unbind(target);
}

SpatialBoundVisual3* RendererDx::createVisualSpatial(const size_t& hash, const Mesh& mesh, const Material& material) {
	return new SpatialBoundVisualDx3(hash, mesh, material);
}

VisualEffect* RendererDx::createVisualEffect(const Effect& effect) {
	return new VisualEffectDx(effect);
}

void RendererDx::draw(SpatialCamera* camera, std::list<SpatialBound3*> bounds, std::vector<VisualEffect*> visualEffects, const std::vector<SpatialLight*>& spatialLights) {
}

void RendererDx::destroyVisualEffect(VisualEffect* visualEffect) {
	VisualEffectDx* visualEffectDx = dynamic_cast<VisualEffectDx*>(visualEffect);
	delete visualEffectDx;
}

void RendererDx::destroyVisualSpatial(SpatialBoundVisual3* visualSpatial) {
}

VisualScreenSpace* RendererDx::createVisualScreenSpace(const Effect& effect) {
	return new VisualScreenSpaceDx(effect);
}

void RendererDx::draw(VisualScreenSpace* visualScreenSpace, Framebuffer* oldFramebuffer, Framebuffer* newFramebuffer, const Mat4& oldViewProjectionStraight, const Mat4& oldViewProjectionInverted, const Mat4& newViewProjectionStraight, const Mat4& newViewProjectionInverted) {
}

void RendererDx::destroyVisualScreenSpace(VisualScreenSpace* visualScreenSpace) {
	VisualScreenSpaceDx* dxVisualScreenSpace = dynamic_cast<VisualScreenSpaceDx*>(visualScreenSpace);
	delete dxVisualScreenSpace;
}

void RendererDx::draw(const Bound3& bound, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, VisualEffect* visualEffect, const std::vector<SpatialLight*>& spatialLights) {
}

Framebuffer* RendererDx::createFramebuffer(const int& width, const int& height, const Framebuffer::Descriptor& descriptor) {
	// TODO
	return nullptr;
}

void RendererDx::destroyFramebuffer(Framebuffer* framebuffer) {
	// TODO
}

#endif // defined(_WIN32)
