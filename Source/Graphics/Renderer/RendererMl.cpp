//
//  RendererMl.cpp
//
//
//  Created by Narendra Umate on 9/7/13.
//
//

#if defined(__APPLE__)

#include "RendererMl.h"

#if defined(__APPLE__)
#include <Dispatch/Dispatch.h>
#include <simd/simd.h>
#endif

RendererMl::RendererMl(const int& width, const int& height)
: Renderer(width, height) {
	m_ndcMatrix = Mat4::translate(Vec3(0.0f, 0.0f, 0.5f)) * Mat4::scale(Vec3(1.0f, 1.0f, 0.5f));
	// initialize
#if defined(__APPLE__)
	m_device = mtlpp::Device::CreateSystemDefaultDevice();
	m_semaphore = dispatch_semaphore_create(1);
	m_commandQueue = m_device.NewCommandQueue();
	m_captureScope = mtlpp::CaptureManager::GetSharedCaptureManager().NewCaptureScope(m_commandQueue);
	mtlpp::CaptureManager::GetSharedCaptureManager().SetDefaultCaptureScope(m_captureScope);
#endif
}

RendererMl::~RendererMl() {
	// deinitialize
}

void RendererMl::clear() {
#if defined(__APPLE__)
	mtlpp::RenderPassDescriptor renderPassDescriptor = m_renderPassDescriptor;
	renderPassDescriptor.GetColorAttachments()[0].SetLoadAction(mtlpp::LoadAction::Clear);
	renderPassDescriptor.GetDepthAttachment().SetLoadAction(mtlpp::LoadAction::Clear);
	renderPassDescriptor.GetStencilAttachment().SetLoadAction(mtlpp::LoadAction::Clear);
	renderPassDescriptor.GetColorAttachments()[0].SetStoreAction(mtlpp::StoreAction::Store);
	renderPassDescriptor.GetDepthAttachment().SetStoreAction(mtlpp::StoreAction::Store);
	renderPassDescriptor.GetStencilAttachment().SetStoreAction(mtlpp::StoreAction::Store);

	mtlpp::CommandBuffer clearCommandBuffer = m_commandQueue.CommandBuffer();
	mtlpp::RenderCommandEncoder renderCommandEncoder = clearCommandBuffer.RenderCommandEncoder(renderPassDescriptor);
	renderCommandEncoder.EndEncoding();
	clearCommandBuffer.Enqueue();
	clearCommandBuffer.Commit();
#endif
}

void RendererMl::setClearColor(const Color4f& clearColor) {
	m_clearColor = clearColor;
}

Color4f RendererMl::getClearColor() const {
	return m_clearColor;
}

void RendererMl::setClearDepth(const float& clearDepth) {
	m_clearDepth = clearDepth;
}

float RendererMl::getClearDepth() const {
	return m_clearDepth;
}

void RendererMl::setClearStencil(const unsigned int& clearStencil) {
	m_clearStencil = clearStencil;
}

unsigned int RendererMl::getClearStencil() const {
	return m_clearStencil;
}

void RendererMl::setViewport(const int& xPos, const int& yPos, const int& w, const int& h) {
#if defined(__APPLE__)
	m_viewport.OriginX = xPos;
	m_viewport.OriginY = yPos;
	m_viewport.Width = w;
	m_viewport.Height = h;
	m_viewport.ZNear = 0;
	m_viewport.ZFar = 1;
#endif
}

void RendererMl::getViewport(int& xPos, int& yPos, int& w, int& h) const {
#if defined(__APPLE__)
	xPos = m_viewport.OriginX / 2;
	yPos = m_viewport.OriginY / 2;
	w = m_viewport.Width / 2;
	h = m_viewport.Height / 2;
#endif
}

void RendererMl::setSize(const int& width, const int& height) {
	m_width = width;
	m_height = height;
}

void RendererMl::getSize(int& width, int& height) {
	width = m_width;
	height = m_height;
}

void RendererMl::blit(const std::vector<Framebuffer*>& framebuffers, const int& cols, const int& rows, const int& width, const int& height, void* texture, void* drawable) {
	if (cols * rows == framebuffers.size()) {
		//int width = 0;
		//int height = 0;
		//m_window->getFramebufferSize(width, height);
		if (width > 0 && height > 0) {
			static unsigned int counter = 0;
			dispatch_semaphore_wait(m_semaphore, DISPATCH_TIME_FOREVER);

			// Create a command buffer for the frame to be rendered this frame

			mtlpp::CommandBuffer blitCommandBuffer = m_commandQueue.CommandBuffer();
#if 0
            blitCommandBuffer.SetLabel(ns::String(std::to_string(counter).c_str()));
#endif
			dispatch_semaphore_t block_sema = m_semaphore;

			// Add completion handler for this command buffer to advance frame
			// You must use this to change the captured class member variables
			blitCommandBuffer.AddCompletedHandler([=](const mtlpp::CommandBuffer& commandBuffer) {
#if 0
				std::cout << commandBuffer.GetLabel().GetCStr() << std::endl;
#endif
				dispatch_semaphore_signal(block_sema);
			});

			mtlpp::BlitCommandEncoder blitCommandEncoder = blitCommandBuffer.BlitCommandEncoder();
			mtlpp::Texture blitTexture = mtlpp::Texture(ns::Handle{ texture });

			mtlpp::Texture colorTexture = dynamic_cast<FramebufferMl*>(framebuffers[0])->m_renderPassDescriptor.GetColorAttachments()[0].GetTexture();

			blitCommandEncoder.Copy(colorTexture, 0, 0, mtlpp::Origin(0, 0, 0), mtlpp::Size(width / 2, height / 2, 1), blitTexture, 0, 0, mtlpp::Origin(0, 0, 0));
			blitCommandEncoder.EndEncoding();
			mtlpp::Drawable blitDrawable = mtlpp::Drawable(ns::Handle{ drawable });
			blitCommandBuffer.Present(blitDrawable);
			blitCommandBuffer.Enqueue();
			blitCommandBuffer.Commit();
			counter++;
		}
	}
}

void RendererMl::beginFrame() {
#if defined(__APPLE__)
	// Temporary begin frame counter.
	static unsigned int counter = 0;

	mtlpp::CaptureManager::GetSharedCaptureManager().GetDefaultCaptureScope().BeginScope();
	//mtlpp::CaptureManager::GetSharedCaptureManager().StartCapture(m_device);

	if (counter < 1 || counter == 6) {
		m_renderPassDescriptor.GetColorAttachments()[0].SetClearColor(mtlpp::ClearColor(1, m_clearColor.g, m_clearColor.b, m_clearColor.a));
		if (counter == 6) {
			counter = 0;
		}
	} else if (counter < 2) {
		m_renderPassDescriptor.GetColorAttachments()[0].SetClearColor(mtlpp::ClearColor(m_clearColor.r, 1, m_clearColor.b, m_clearColor.a));
	} else if (counter < 3) {
		m_renderPassDescriptor.GetColorAttachments()[0].SetClearColor(mtlpp::ClearColor(m_clearColor.r, m_clearColor.g, 1, m_clearColor.a));
	} else if (counter < 4) {
		m_renderPassDescriptor.GetColorAttachments()[0].SetClearColor(mtlpp::ClearColor(m_clearColor.r, 1, 1, m_clearColor.a));
	} else if (counter < 5) {
		m_renderPassDescriptor.GetColorAttachments()[0].SetClearColor(mtlpp::ClearColor(1, m_clearColor.g, 1, m_clearColor.a));
	} else if (counter < 6) {
		m_renderPassDescriptor.GetColorAttachments()[0].SetClearColor(mtlpp::ClearColor(1, 1, m_clearColor.b, m_clearColor.a));
	}

	ProjectionRange projectionRange = m_projectionRange;
	if (projectionRange == ForwardFiniteZ || projectionRange == ForwardInfiniteZ) {
		m_depthCompareFunction = mtlpp::CompareFunction::Less;
		//glDepthRange(0.0, 1.0);
		m_renderPassDescriptor.GetDepthAttachment().SetClearDepth(m_clearDepth);
	} else {
		m_depthCompareFunction = mtlpp::CompareFunction::Greater;
		//glDepthRange(1.0, 0.0);
		m_renderPassDescriptor.GetDepthAttachment().SetClearDepth(1.0f - m_clearDepth);
	}

	// Increment begin frame counter.
	counter++;
#endif
}

void RendererMl::endFrame() {
#if defined(__APPLE__)
	//mtlpp::CaptureManager::GetSharedCaptureManager().StopCapture();
	mtlpp::CaptureManager::GetSharedCaptureManager().GetDefaultCaptureScope().EndScope();
#endif
}

void RendererMl::bind(Framebuffer* framebuffer, const Framebuffer::Target& target) {
	framebuffer->bind(target);
	mtlpp::Texture color = dynamic_cast<FramebufferMl*>(framebuffer)->m_renderPassDescriptor.GetColorAttachments()[0].GetTexture();
	mtlpp::Texture depth = dynamic_cast<FramebufferMl*>(framebuffer)->m_renderPassDescriptor.GetDepthAttachment().GetTexture();
	mtlpp::Texture stencil = dynamic_cast<FramebufferMl*>(framebuffer)->m_renderPassDescriptor.GetStencilAttachment().GetTexture();
	m_renderPassDescriptor.GetColorAttachments()[0].SetTexture(color);
	m_renderPassDescriptor.GetDepthAttachment().SetTexture(depth);
	m_renderPassDescriptor.GetStencilAttachment().SetTexture(stencil);
}

void RendererMl::unbind(Framebuffer* framebuffer, const Framebuffer::Target& target) {
	mtlpp::Texture color;
	mtlpp::Texture depth;
	mtlpp::Texture stencil;
	m_renderPassDescriptor.GetColorAttachments()[0].SetTexture(color);
	m_renderPassDescriptor.GetDepthAttachment().SetTexture(depth);
	m_renderPassDescriptor.GetStencilAttachment().SetTexture(stencil);
	framebuffer->unbind(target);
}

SpatialBoundVisual3* RendererMl::createVisualSpatial(const Mesh& mesh, const Material& material) {
	return new SpatialBoundVisualMl3(mesh, material);
}

VisualEffect* RendererMl::createVisualEffect(const Effect& effect) {
	return new VisualEffectMl(effect);
}

void RendererMl::draw(SpatialBoundVisual3* visual, VisualEffect* visualEffect, const Vec3& eyePosition, const std::vector<SpatialLight*>& spatialLights) {
#if defined(__APPLE__)
	mtlpp::RenderPassDescriptor renderPassDescriptor = m_renderPassDescriptor;
	renderPassDescriptor.GetColorAttachments()[0].SetLoadAction(mtlpp::LoadAction::Load);
	renderPassDescriptor.GetDepthAttachment().SetLoadAction(mtlpp::LoadAction::Load);
	renderPassDescriptor.GetStencilAttachment().SetLoadAction(mtlpp::LoadAction::Load);
	renderPassDescriptor.GetColorAttachments()[0].SetStoreAction(mtlpp::StoreAction::Store);
	renderPassDescriptor.GetDepthAttachment().SetStoreAction(mtlpp::StoreAction::Store);
	renderPassDescriptor.GetStencilAttachment().SetStoreAction(mtlpp::StoreAction::Store);

	mtlpp::CommandBuffer drawCommandBuffer = m_commandQueue.CommandBuffer();
	mtlpp::RenderCommandEncoder renderCommandEncoder = drawCommandBuffer.RenderCommandEncoder(renderPassDescriptor);

	//renderCommandEncoder.SetViewport(m_viewport);
	//renderCommandEncoder.SetCullMode(mtlpp::CullMode::Back);
	//renderCommandEncoder.SetFrontFacingWinding(mtlpp::Winding::CounterClockwise);

	mtlpp::RenderPipelineDescriptor renderPipelineDescriptor;
	renderPipelineDescriptor.GetColorAttachments()[0].SetPixelFormat(renderPassDescriptor.GetColorAttachments()[0].GetTexture().GetPixelFormat());
	renderPipelineDescriptor.SetDepthAttachmentPixelFormat(renderPassDescriptor.GetDepthAttachment().GetTexture().GetPixelFormat());
	renderPipelineDescriptor.SetStencilAttachmentPixelFormat(renderPassDescriptor.GetStencilAttachment().GetTexture().GetPixelFormat());

	VisualEffectMl* visualEffectMl = dynamic_cast<VisualEffectMl*>(visualEffect);
	renderPipelineDescriptor.SetVertexFunction(visualEffectMl->m_vertexFunction);
	renderPipelineDescriptor.SetFragmentFunction(visualEffectMl->m_fragmentFunction);

	mtlpp::RenderPipelineState renderPipelineState = m_device.NewRenderPipelineState(renderPipelineDescriptor, nullptr);
	renderCommandEncoder.SetRenderPipelineState(renderPipelineState);

	mtlpp::DepthStencilDescriptor depthStencilDescriptor;
	depthStencilDescriptor.SetDepthCompareFunction(m_depthCompareFunction);
	depthStencilDescriptor.SetDepthWriteEnabled(true);

	mtlpp::DepthStencilState depthStencilState = m_device.NewDepthStencilState(depthStencilDescriptor);
	renderCommandEncoder.SetDepthStencilState(depthStencilState);

	// Uniform
	{
		typedef struct
		{
			matrix_float4x4 wvp_straight;
		} Uniform;
		Mat4 wvp_straight_m = visual->getWorldViewProjectionMatrix();
		const Uniform uniforms[] = {
			{ .wvp_straight = (matrix_float4x4){
				  { { wvp_straight_m.r[0], wvp_straight_m.r[1], wvp_straight_m.r[2], wvp_straight_m.r[3] },
					  { wvp_straight_m.r[4], wvp_straight_m.r[5], wvp_straight_m.r[6], wvp_straight_m.r[7] },
					  { wvp_straight_m.r[8], wvp_straight_m.r[9], wvp_straight_m.r[10], wvp_straight_m.r[11] },
					  { wvp_straight_m.r[12], wvp_straight_m.r[13], wvp_straight_m.r[14], wvp_straight_m.r[15] } } } }
		};
		renderCommandEncoder.SetVertexData(uniforms, sizeof(uniforms), 1);
	}

	// Vertex
	{
		VertexBufferMl* vertexBufferMl = dynamic_cast<VertexBufferMl*>(visual->getVertexBuffer());
		renderCommandEncoder.SetVertexBuffer(vertexBufferMl->m_vertexBuffer, 0, 0);
	}

	// Index
	{
		unsigned int ica = visual->getIndexBuffer(0) ? visual->getIndexBuffer(0)->getIndexCount() : 0;
		unsigned int icb = visual->getIndexBuffer(1) ? visual->getIndexBuffer(1)->getIndexCount() : 0;
		unsigned int icc = visual->getIndexBuffer(2) ? visual->getIndexBuffer(2)->getIndexCount() : 0;
		unsigned int icd = visual->getIndexBuffer(3) ? visual->getIndexBuffer(3)->getIndexCount() : 0;
		unsigned int ice = visual->getIndexBuffer(4) ? visual->getIndexBuffer(4)->getIndexCount() : 0;
		unsigned int icf = std::max(ica, std::max(icb, std::max(icc, std::max(icd, ice))));

		IndexBuffer* iba = visual->getIndexBuffer(0);
		IndexBuffer* ibb = visual->getIndexBuffer(1);
		IndexBuffer* ibc = visual->getIndexBuffer(2);
		IndexBuffer* ibd = visual->getIndexBuffer(3);
		IndexBuffer* ibe = visual->getIndexBuffer(4);
		IndexBuffer* ibf = iba ? iba : (ibb ? ibb : (ibc ? ibc : (ibd ? ibd : ibe)));

		unsigned int indexCount = icf;

		IndexBufferMl* indexBufferMl = dynamic_cast<IndexBufferMl*>(ibf);
		renderCommandEncoder.DrawIndexed(mtlpp::PrimitiveType::Triangle, indexCount, mtlpp::IndexType::UInt32, indexBufferMl->m_indexBuffer, 0);
	}

	renderCommandEncoder.EndEncoding();

	drawCommandBuffer.Enqueue();
	drawCommandBuffer.Commit();
#endif
}

void RendererMl::destroyVisualEffect(VisualEffect* visualEffect) {
	VisualEffectMl* visualEffectMl = dynamic_cast<VisualEffectMl*>(visualEffect);
	delete visualEffectMl;
}

void RendererMl::destroyVisualSpatial(SpatialBoundVisual3* visualSpatial) {
	SpatialBoundVisualMl3* mlVisualSpatial = dynamic_cast<SpatialBoundVisualMl3*>(visualSpatial);
	delete mlVisualSpatial;
}

VisualScreenSpace* RendererMl::createVisualScreenSpace(const Effect& effect) {
	return new MlVisualScreenSpace(effect);
}

void RendererMl::draw(VisualScreenSpace* visualScreenSpace, Framebuffer* oldFramebuffer, Framebuffer* newFramebuffer, const Mat4& oldViewProjectionStraight, const Mat4& oldViewProjectionInverted, const Mat4& newViewProjectionStraight, const Mat4& newViewProjectionInverted) {
}

void RendererMl::destroyVisualScreenSpace(VisualScreenSpace* visualScreenSpace) {
	MlVisualScreenSpace* mlVisualScreenSpace = dynamic_cast<MlVisualScreenSpace*>(visualScreenSpace);
	delete mlVisualScreenSpace;
}

void RendererMl::draw(const Bound3& bound, VisualEffect* visualEffect, VisualMaterial* visualMaterial, const Mat4& worldMatrix, const Mat4& viewMatrix, const Mat4& viewProjectionMatrix, const Vec3& lightCoefficients, const Vec3& lightColor, const Vec3& lightPosition, const Vec3& eyePosition) {
}

Framebuffer* RendererMl::createFramebuffer(const int& width, const int& height, const Framebuffer::Descriptor& descriptor) {
	return new FramebufferMl(width, height, descriptor);
}

void RendererMl::destroyFramebuffer(Framebuffer* framebuffer) {
	FramebufferMl* mlFramebuffer = dynamic_cast<FramebufferMl*>(framebuffer);
	delete mlFramebuffer;
}

#endif // defined(__APPLE__)
