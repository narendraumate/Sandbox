//
//  RendererGl.h
//
//
//  Created by Narendra Umate on 9/7/13.
//
//

#pragma once

#if defined(__APPLE__) || defined(__linux__) || defined(_WIN32)

#define BUFFER_OFFSET(offset) (void*)((char*)nullptr + offset)

#include "BufferGl.h"
#include "Renderer.h"
#include "SpatialBoundVisualGl.h"
#include "VisualScreenSpaceGl.h"

class RendererGl : public Renderer {

public:
	RendererGl(const int& width, const int& height);
	virtual ~RendererGl();

	void clear();
	void setClearColor(const Color4f& clearColor);
	Color4f getClearColor() const;
	void setClearDepth(const float& clearDepth);
	float getClearDepth() const;
	void setClearStencil(const unsigned int& clearStencil);
	unsigned int getClearStencil() const;
	void setViewport(const int& xPos, const int& yPos, const int& w, const int& h);
	void getViewport(int& xPos, int& yPos, int& w, int& h) const;
	void setSize(const int& width, const int& height);
	void getSize(int& width, int& height);

	void blit(const std::vector<Framebuffer*>& framebuffers, const int& cols, const int& rows, const int& width, const int& height, void* texture, void* drawable);

	void beginFrame();
	void endFrame();

	void bind(Framebuffer* framebuffer, const Framebuffer::Target& target);
	void unbind(Framebuffer* framebuffer, const Framebuffer::Target& target);

	SpatialBoundVisual3* createVisualSpatial(const size_t& hash, const Mesh& mesh, const Material& material);
	VisualEffect* createVisualEffect(const Effect& effect);
	void draw(SpatialCamera* camera, std::list<SpatialBound3*> bounds, std::vector<VisualEffect*> visualEffects, const std::vector<SpatialLight*>& spatialLights);
	void destroyVisualEffect(VisualEffect* visualEffect);
	void destroyVisualSpatial(SpatialBoundVisual3* visual);

	VisualScreenSpace* createVisualScreenSpace(const Effect& effect);
	void draw(VisualScreenSpace* visualScreenSpace, Framebuffer* oldFramebuffer, Framebuffer* newFramebuffer, const Mat4& oldViewProjectionStraight, const Mat4& oldViewProjectionInverted, const Mat4& newViewProjectionStraight, const Mat4& newViewProjectionInverted);
	void destroyVisualScreenSpace(VisualScreenSpace* visualScreenSpace);

	void draw(const Bound3& bound, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, VisualEffect* visualEffect, const std::vector<SpatialLight*>& spatialLights);

	Framebuffer* createFramebuffer(const int& width, const int& height, const Framebuffer::Descriptor& descriptor);
	void destroyFramebuffer(Framebuffer* framebuffer);

private:
	// HACK Make this into associated class pointers that are automatically released
	VisualScreenSpace* m_quadVisualScreenSpace;
};

#endif // defined(__APPLE__) || defined(__linux__) || defined (_WIN32)
