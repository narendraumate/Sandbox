//
//  Renderer.h
//
//
//  Created by Narendra Umate on 9/7/13.
//
//

#pragma once

#include "AbstractWindow.h"
#include "Color.h"
#include "Matrix.h"
#include "Projection.h"
#include "SpatialBoundVisual.h"
#include "SpatialLight.h"
#include "VisualScreenSpace.h"

class AbstractWindow;

class Renderer {
public:
	Renderer(const int& width, const int& height);
	virtual ~Renderer() = 0;

	void setProjectionDimension(const ProjectionDimension& dimension);
	ProjectionDimension getProjectionDimension();

	void setProjectionDirection(const ProjectionDirection& direction);
	ProjectionDirection getProjectionDirection();

	void setWidth(const int& width);
	int getWidth();

	void setHeight(const int& height);
	int getHeight();

	void draw(Bvh* bvh, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, VisualEffect* visualEffect, const std::vector<SpatialLight*>& spatialLights);
	void draw(Octree* octree, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, VisualEffect* visualEffect, const std::vector<SpatialLight*>& spatialLights);

	virtual void draw(const Bound3& bound, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, VisualEffect* visualEffect, const std::vector<SpatialLight*>& spatialLights) = 0;

	virtual void clear() = 0;
	virtual void setClearColor(const Color4f& clearColor) = 0;
	virtual Color4f getClearColor() const = 0;
	virtual void setClearDepth(const float& clearDepth) = 0;
	virtual float getClearDepth() const = 0;
	virtual void setClearStencil(const unsigned int& clearStencil) = 0;
	virtual unsigned int getClearStencil() const = 0;
	virtual void setViewport(const int& xPos, const int& yPos, const int& w, const int& h) = 0;
	virtual void getViewport(int& xPos, int& yPos, int& w, int& h) const = 0;
	virtual void setSize(const int& width, const int& height) = 0;
	virtual void getSize(int& width, int& height) = 0;

	virtual void blit(const std::vector<Framebuffer*>& framebuffers, const int& cols, const int& rows, const int& width, const int& height, void* texture, void* drawable) = 0;

	virtual void beginFrame() = 0;
	virtual void endFrame() = 0;

	virtual void bind(Framebuffer* framebuffer, const Framebuffer::Target& target) = 0;
	virtual void unbind(Framebuffer* framebuffer, const Framebuffer::Target& target) = 0;

	virtual SpatialBoundVisual3* createVisualSpatial(const size_t& hash, const Mesh& mesh, const Material& material) = 0;
	virtual VisualEffect* createVisualEffect(const Effect& effect) = 0;
	virtual void draw(SpatialCamera* camera, std::list<SpatialBound3*> bounds, std::vector<VisualEffect*> visualEffects, const std::vector<SpatialLight*>& spatialLights) = 0;
	virtual void destroyVisualEffect(VisualEffect* visualEffect) = 0;
	virtual void destroyVisualSpatial(SpatialBoundVisual3* visual) = 0;

	virtual VisualScreenSpace* createVisualScreenSpace(const Effect& effect) = 0;
	virtual void draw(VisualScreenSpace* visualScreenSpace, Framebuffer* oldFramebuffer, Framebuffer* newFramebuffer, const Mat4& oldViewProjectionStraight, const Mat4& oldViewProjectionInverted, const Mat4& newViewProjectionStraight, const Mat4& newViewProjectionInverted) = 0;
	virtual void destroyVisualScreenSpace(VisualScreenSpace* visual) = 0;

	virtual Framebuffer* createFramebuffer(const int& width, const int& height, const Framebuffer::Descriptor& descriptor) = 0;
	virtual void destroyFramebuffer(Framebuffer* framebuffer) = 0;

protected:
	ProjectionDimension m_projectionDimension;
	ProjectionDirection m_projectionDirection;

	int m_width;
	int m_height;
	Color4f m_clearColor;
	float m_clearDepth;
	unsigned int m_clearStencil;
};
