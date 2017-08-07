//
//  RendererVk.cpp
//
//
//  Created by Narendra Umate on 9/7/13.
//
//

#if defined(__APPLE__) || defined(__linux__) || defined(_WIN32)

#include "RendererVk.h"

RendererVk::RendererVk(const int& width, const int& height)
: Renderer(width, height) {
	m_projectionDimension = X2Y2Z1;
	// initialize
	////vkEnable(GL_DEPTH_TEST);
	////vkDepthFunc(GL_LESS);
	////vkEnable(GL_CULL_FACE);
	////vkCullFace(GL_BACK);
	////vkFrontFace(GL_CW); // GL_CW or GL_CCW
	////vkPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_FILL or GL_LINE
	////vkClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	////vkClearDepth(m_clearDepth);
	////vkClearStencil(m_clearStencil);
	////vkClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

RendererVk::~RendererVk() {
}

void RendererVk::clear() {
	// vkClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
	// vkClearDepth(m_clearDepth);
	// vkClearStencil(m_clearStencil);
	////vkClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RendererVk::setClearColor(const Color4f& clearColor) {
	m_clearColor = clearColor;
	////vkClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
}

Color4f RendererVk::getClearColor() const {
	return m_clearColor;
}

void RendererVk::setClearDepth(const float& clearDepth) {
	m_clearDepth = clearDepth;
	////vkClearDepth(m_clearDepth);
}

float RendererVk::getClearDepth() const {
	return m_clearDepth;
}

void RendererVk::setClearStencil(const unsigned int& clearStencil) {
	m_clearStencil = clearStencil;
}

unsigned int RendererVk::getClearStencil() const {
	return m_clearStencil;
}

void RendererVk::setViewport(const int& xPos, const int& yPos, const int& w, const int& h) {
}

void RendererVk::getViewport(int& xPos, int& yPos, int& w, int& h) const {
}

void RendererVk::setSize(const int& width, const int& height) {
	m_width = width;
	m_height = height;
}

void RendererVk::getSize(int& width, int& height) {
	width = m_width;
	height = m_height;
}

void RendererVk::blit(const std::vector<Framebuffer*>& framebuffers, const int& cols, const int& rows, const int& width, const int& height, void* texture, void* drawable) {
}

void RendererVk::beginFrame() {
}

void RendererVk::endFrame() {
}

void RendererVk::bind(Framebuffer* framebuffer, const Framebuffer::Target& target) {
	framebuffer->bind(target);
}

void RendererVk::unbind(Framebuffer* framebuffer, const Framebuffer::Target& target) {
	framebuffer->unbind(target);
}

SpatialBoundVisual3* RendererVk::createVisualSpatial(const size_t& hash, const Mesh& mesh, const Material& material) {
	return new SpatialBoundVisualVk3(hash, mesh, material);
}

VisualEffect* RendererVk::createVisualEffect(const Effect& effect) {
	return new VisualEffectVk(effect);
}

void RendererVk::draw(SpatialCamera* camera, std::list<SpatialBound3*> bounds, std::vector<VisualEffect*> visualEffects, const std::vector<SpatialLight*>& spatialLights) {
	size_t visualEffectIndex = 0;
	for (auto bound : bounds) {
		VisualEffect* visualEffect = visualEffects.at(visualEffectIndex++ % visualEffects.size());
		SpatialBoundVisual3* visual = dynamic_cast<SpatialBoundVisual3*>(bound);
		visual->setViewProjectionMatrix(camera->getViewProjectionMatrix(), camera->getProjectionMatrix());

		VisualEffectVk* vkVisualEffect = dynamic_cast<VisualEffectVk*>(visualEffect);
		VisualMaterialVk* vkVisualMaterial = dynamic_cast<VisualMaterialVk*>(visual->getVisualMaterial());
		VertexBufferVk* vkVertexBuffer = dynamic_cast<VertexBufferVk*>(visual->getVertexBuffer());
		unsigned int visualIndexBufferCount = visual->getIndexBufferCount();
		if (vkVisualMaterial && vkVisualEffect && vkVertexBuffer && visualIndexBufferCount) {
			// Enable Effect.
			// vkVisualEffect->enable();
			// Set light and eye parameters.
			////int lightCount = 1;
			////vkVisualEffect->set1iv("lightCount", 1, &lightCount);
			////vkVisualEffect->set3fv("lightCoefficients", lightCount, lightCoefficients.data);
			////vkVisualEffect->set3fv("lightColor", lightCount, lightColor.data);
			////vkVisualEffect->set3fv("lightPosition", lightCount, lightPosition.data);
			Mat4 inverseViewMatrix = camera->getTransform();
			vkVisualEffect->set3fv("eyePosition", 1, &inverseViewMatrix.d[3][0]);
			// Set World View Norm Matrix.
			vkVisualEffect->setMatrix3fv("worldNormal", 1, false, (float*)&(visual->getWorldNormalMatrix()));
			// Set World View Projection Matrix.
			vkVisualEffect->setMatrix4fv("worldViewProjection", 1, false, (float*)&(visual->getWorldViewProjectionMatrix()));
			// Set Parameters.
			vkVisualEffect->set3fv("ambient", 1, vkVisualMaterial->ambient);
			vkVisualEffect->set3fv("diffuse", 1, vkVisualMaterial->diffuse);
			vkVisualEffect->set3fv("specular", 1, vkVisualMaterial->specular);
			vkVisualEffect->set3fv("transmittance", 1, vkVisualMaterial->transmittance);
			vkVisualEffect->set3fv("emission", 1, vkVisualMaterial->emission);
			vkVisualEffect->set1fv("shininess", 1, &vkVisualMaterial->shininess);
			vkVisualEffect->set1fv("ior", 1, &vkVisualMaterial->ior);
			// Enable Material.
			vkVisualMaterial->enable();
			////////////////////////////////////////////////////////////////////////
			// Bind Vertex Buffer.
			vkVertexBuffer->bind();
			for (int indexBufferIndex = 0; indexBufferIndex < 5; ++indexBufferIndex) {
				IndexBuffer* indexBuffer = visual->getIndexBuffer(indexBufferIndex);
				if (indexBuffer) {
					IndexBufferVk* vkIndexBuffer = dynamic_cast<IndexBufferVk*>(indexBuffer);
					if (vkIndexBuffer) {
						// Bind Index Buffer.
						vkIndexBuffer->bind();
						switch (vkIndexBuffer->getType()) {
							case IndexBuffer::Point:
								////vkDrawElements(GL_POINTS, static_cast<int>(vkIndexBuffer->getIndexCount()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
								break;
							case IndexBuffer::Line:
								////vkDrawElements(GL_LINES, static_cast<int>(vkIndexBuffer->getIndexCount()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
								break;
							case IndexBuffer::Triangle:
								////vkDrawElements(GL_TRIANGLES, static_cast<int>(vkIndexBuffer->getIndexCount()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
								break;
							default:
								break;
						}
						// Unbind Index Buffer.
						vkIndexBuffer->unbind();
					}
				}
			}
			// Unbind Vertex Buffer.
			vkVertexBuffer->unbind();
			////////////////////////////////////////////////////////////////////////
			// Disable Material.
			vkVisualMaterial->disable();
			// Disable Effect.
			// vkVisualEffect->disable();
		}
	}
}

void RendererVk::destroyVisualEffect(VisualEffect* visualEffect) {
	VisualEffectVk* visualEffectVk = dynamic_cast<VisualEffectVk*>(visualEffect);
	delete visualEffectVk;
}

void RendererVk::destroyVisualSpatial(SpatialBoundVisual3* visualSpatial) {
	SpatialBoundVisualVk3* vkVisualSpatial = dynamic_cast<SpatialBoundVisualVk3*>(visualSpatial);
	delete vkVisualSpatial;
}

VisualScreenSpace* RendererVk::createVisualScreenSpace(const Effect& effect) {
	return new VisualScreenSpaceVk(effect);
}

void RendererVk::draw(VisualScreenSpace* visualScreenSpace, Framebuffer* oldFramebuffer, Framebuffer* newFramebuffer, const Mat4& oldViewProjectionStraight, const Mat4& oldViewProjectionInverted, const Mat4& newViewProjectionStraight, const Mat4& newViewProjectionInverted) {
}

void RendererVk::destroyVisualScreenSpace(VisualScreenSpace* visualScreenSpace) {
	VisualScreenSpaceVk* vkVisualScreenSpace = dynamic_cast<VisualScreenSpaceVk*>(visualScreenSpace);
	delete vkVisualScreenSpace;
}

void RendererVk::draw(const Bound3& bound, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, VisualEffect* visualEffect, const std::vector<SpatialLight*>& spatialLights) {
#if 0
	// Initialize resources at highest level.
	if (true) {
		Mat3 worldNormalMatrix = Mat3(worldMatrix).inverse().transpose();
		Mat4 worldViewProjectionMatrix = viewProjectionMatrix * worldMatrix;

		VisualEffectVk* vkVisualEffect = dynamic_cast<VisualEffectVk*>(visualEffect);
		VisualMaterialVk* vkVisualMaterial = dynamic_cast<VisualMaterialVk*>(visualMaterial);

		// Enable Effect.
		////vkVisualEffect->enable();
		// Set light and eye parameters.
		int lightCount = 1;
		vkVisualEffect->set1iv("lightCount", 1, &lightCount);
		vkVisualEffect->set3fv("lightCoefficients", 1, lightCoefficients.data);
		vkVisualEffect->set3fv("lightColor", 1, lightColor.data);
		vkVisualEffect->set3fv("lightPosition", 1, lightPosition.data);
		vkVisualEffect->set3fv("eyePosition", 1, eyePosition.data);
		// Set World View Norm Matrix.
		vkVisualEffect->setMatrix3fv("worldNormal", 1, false, (float*)&(worldNormalMatrix));
		// Set World View Projection Matrix.
		vkVisualEffect->setMatrix4fv("worldViewProjection", 1, false, (float*)&(worldViewProjectionMatrix));
		// Set Parameters.
		vkVisualEffect->set3fv("ambient", 1, vkVisualMaterial->ambient);
		vkVisualEffect->set3fv("diffuse", 1, vkVisualMaterial->diffuse);
		vkVisualEffect->set3fv("specular", 1, vkVisualMaterial->specular);
		vkVisualEffect->set3fv("transmittance", 1, vkVisualMaterial->transmittance);
		vkVisualEffect->set3fv("emission", 1, vkVisualMaterial->emission);
		vkVisualEffect->set1fv("shininess", 1, &vkVisualMaterial->shininess);
		vkVisualEffect->set1fv("ior", 1, &vkVisualMaterial->ior);
		// Enable Material.
		vkVisualMaterial->enable();
	}

	// Draw your self.
	{
		// Scoped this draw to destroy buffers right away.
		std::vector<P4N4T4B4T2P2> vertices;
		bound.getCornerVertices(vertices);
		VertexBufferVk vkVertexBuffer(vertices);
		std::vector<unsigned int> indices;
		bound.getCornerIndices(indices);
		IndexBufferVk vkIndexBuffer(IndexBufferVk::Line, indices);
		vkVertexBuffer.bind();
		vkIndexBuffer.bind();
		////vkDrawElements(GL_LINES, static_cast<int>(vkIndexBuffer.getIndexCount()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		vkIndexBuffer.unbind();
		vkVertexBuffer.unbind();
	}

	// Deinitialize resources at highest level.
	if (true) {
		// Disable Material.
		VisualMaterialVk* vkVisualMaterial = dynamic_cast<VisualMaterialVk*>(visualMaterial);
		vkVisualMaterial->disable();

		// Disable Effect.
		// VisualEffectVk* vkVisualEffect = dynamic_cast<VisualEffectVk*>(visualEffect);
		// vkVisualEffect->disable();
	}
#endif
}

Framebuffer* RendererVk::createFramebuffer(const int& width, const int& height, const Framebuffer::Descriptor& descriptor) {
	return new FramebufferVk(width, height, descriptor);
}

void RendererVk::destroyFramebuffer(Framebuffer* framebuffer) {
	FramebufferVk* vkFramebuffer = dynamic_cast<FramebufferVk*>(framebuffer);
	delete vkFramebuffer;
}

#endif // defined(__APPLE__) || defined(__linux__) || defined(_WIN32)
