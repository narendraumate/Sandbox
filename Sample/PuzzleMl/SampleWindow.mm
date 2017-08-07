//
//  SampleWindow.cpp
//
//
//  Created by Narendra Umate on 8/18/15.
//
//

#include "SampleWindow.h"

#if defined(__APPLE__)
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#endif

SampleWindow::SampleWindow(Parameters& parameters)
: BaseWindow(parameters) {
#if defined(__APPLE__)

    const char shadersSrc[] = R"""(
        #include <metal_stdlib>
        using namespace metal;
        vertex float4 vertFunc(
            const device packed_float3* vertexArray [[buffer(0)]],
            unsigned int vID[[vertex_id]])
        {
            return float4(vertexArray[vID], 1.0);
        }
        fragment half4 fragFunc()
        {
            return half4(1.0, 0.0, 0.0, 1.0);
        }
    )""";

	const float vertexData[] = {
		0.0f,
		1.0f,
		0.0f,
		-1.0f,
		-1.0f,
		0.0f,
		1.0f,
		-1.0f,
		0.0f,
	};

#if USE_MTLPP

    m_device = mtlpp::Device::CreateSystemDefaultDevice();
	m_commandQueue = m_device.NewCommandQueue();
    m_vertexBuffer = m_device.NewBuffer(vertexData, sizeof(vertexData), mtlpp::ResourceOptions::CpuCacheModeDefaultCache);

    mtlpp::Library library = m_device.NewLibrary(shadersSrc, mtlpp::CompileOptions(), nullptr);
	mtlpp::Function vertFunc = library.NewFunction("vertFunc");
	mtlpp::Function fragFunc = library.NewFunction("fragFunc");

	mtlpp::RenderPipelineDescriptor renderPipelineDesc;
	renderPipelineDesc.SetVertexFunction(vertFunc);
	renderPipelineDesc.SetFragmentFunction(fragFunc);
	renderPipelineDesc.GetColorAttachments()[0].SetPixelFormat(mtlpp::PixelFormat::BGRA8Unorm_sRGB);
	renderPipelineDesc.SetDepthAttachmentPixelFormat(mtlpp::PixelFormat::Depth32Float_Stencil8);
	renderPipelineDesc.SetStencilAttachmentPixelFormat(mtlpp::PixelFormat::Depth32Float_Stencil8);
	m_renderPipelineState = m_device.NewRenderPipelineState(renderPipelineDesc, nullptr);

#else

    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    m_device = (void*)CFBridgingRetain(device);
    id<MTLCommandQueue> commandQueue = [device newCommandQueue];
    m_commandQueue = (void*)CFBridgingRetain(commandQueue);
    id<MTLBuffer> vertexBuffer = [device newBufferWithBytes:vertexData
                                                     length:sizeof(vertexData)
                                                    options:MTLResourceCPUCacheModeDefaultCache];
    m_vertexBuffer = (void*)CFBridgingRetain(vertexBuffer);

    NSError* libraryError = nullptr;
    MTLCompileOptions* compileOptions = [MTLCompileOptions new];
    id<MTLLibrary> library = [device newLibraryWithSource:[NSString stringWithUTF8String:shadersSrc]
                                                  options:compileOptions
                                                    error:&libraryError];
    [compileOptions release];
    id<MTLFunction> vertFunc = [library newFunctionWithName:@"vertFunc"];
    id<MTLFunction> fragFunc = [library newFunctionWithName:@"fragFunc"];

    MTLRenderPipelineDescriptor* renderPipelineDesc = [MTLRenderPipelineDescriptor new];
    renderPipelineDesc.vertexFunction = vertFunc;
    renderPipelineDesc.fragmentFunction = fragFunc;
    [vertFunc release];
    [fragFunc release];
    [library release];
    renderPipelineDesc.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
    renderPipelineDesc.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    renderPipelineDesc.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    NSError* renderPipelineStateError = nullptr;
    id<MTLRenderPipelineState> renderPipelineState = [device newRenderPipelineStateWithDescriptor:renderPipelineDesc
                                                                                            error:&renderPipelineStateError];
    [renderPipelineDesc release];
    m_renderPipelineState = (void*)CFBridgingRetain(renderPipelineState);

#endif

    m_semaphore = dispatch_semaphore_create(1);

#endif
}

SampleWindow::~SampleWindow() {
}

void SampleWindow::onResize(const int& xSize, const int& ySize) {
}

void SampleWindow::onFocus(const int& focus) {
}

void SampleWindow::update(float dt) {
#if defined(__APPLE__)

#if USE_MTLPP

    // color
	mtlpp::TextureDescriptor colorTextureDescriptor = mtlpp::TextureDescriptor::Texture2DDescriptor(mtlpp::PixelFormat::BGRA8Unorm_sRGB, 800, 600, false);
	unsigned int colorTextureUsage = 0;
	colorTextureUsage |= static_cast<unsigned int>(mtlpp::TextureUsage::RenderTarget);
	colorTextureUsage |= static_cast<unsigned int>(mtlpp::TextureUsage::ShaderRead);
	colorTextureDescriptor.SetUsage((mtlpp::TextureUsage)colorTextureUsage);
	mtlpp::Texture colorTexture = m_device.NewTexture(colorTextureDescriptor);
	colorTextureDescriptor.Release();
	// depth
	mtlpp::TextureDescriptor depthTextureDescriptor = mtlpp::TextureDescriptor::Texture2DDescriptor(mtlpp::PixelFormat::Depth32Float_Stencil8, 800, 600, false);
	unsigned int depthResourceOptions = 0;
	depthResourceOptions |= static_cast<unsigned int>(mtlpp::ResourceOptions::StorageModePrivate);
	depthTextureDescriptor.SetResourceOptions((mtlpp::ResourceOptions)depthResourceOptions);
	unsigned int depthTextureUsage = 0;
	depthTextureUsage |= static_cast<unsigned int>(mtlpp::TextureUsage::RenderTarget);
	depthTextureUsage |= static_cast<unsigned int>(mtlpp::TextureUsage::ShaderRead);
	depthTextureDescriptor.SetUsage((mtlpp::TextureUsage)depthTextureUsage);
	mtlpp::Texture depthTexture = m_device.NewTexture(depthTextureDescriptor);
	depthTextureDescriptor.Release();
	// draw render pass descriptor
	mtlpp::RenderPassDescriptor renderPassDescriptor;
	renderPassDescriptor.GetColorAttachments()[0].SetTexture(colorTexture);
	renderPassDescriptor.GetColorAttachments()[0].SetLoadAction(mtlpp::LoadAction::Clear);
	renderPassDescriptor.GetColorAttachments()[0].SetStoreAction(mtlpp::StoreAction::Store);
	renderPassDescriptor.GetColorAttachments()[0].SetClearColor(mtlpp::ClearColor(rand(0.0f, 1.0f), rand(0.0f, 1.0f), rand(0.0f, 1.0f), rand(0.0f, 1.0f)));
	renderPassDescriptor.GetDepthAttachment().SetTexture(depthTexture);
	renderPassDescriptor.GetDepthAttachment().SetLoadAction(mtlpp::LoadAction::Clear);
	renderPassDescriptor.GetDepthAttachment().SetStoreAction(mtlpp::StoreAction::Store);
	renderPassDescriptor.GetDepthAttachment().SetClearDepth(1.0);
	renderPassDescriptor.GetStencilAttachment().SetTexture(depthTexture);
	renderPassDescriptor.GetStencilAttachment().SetLoadAction(mtlpp::LoadAction::Clear);
	renderPassDescriptor.GetStencilAttachment().SetStoreAction(mtlpp::StoreAction::Store);
	// draw command buffer
	mtlpp::CommandBuffer renderCommandBuffer = m_commandQueue.CommandBuffer();
	mtlpp::RenderCommandEncoder renderCommandEncoder = renderCommandBuffer.RenderCommandEncoder(renderPassDescriptor);
	renderPassDescriptor.Release();
	renderCommandEncoder.SetRenderPipelineState(m_renderPipelineState);
	renderCommandEncoder.SetVertexBuffer(m_vertexBuffer, 0, 0);
	renderCommandEncoder.Draw(mtlpp::PrimitiveType::Triangle, 0, 3);
	renderCommandEncoder.EndEncoding();
	renderCommandEncoder.Release();
	renderCommandBuffer.Enqueue();
	renderCommandBuffer.Commit();
	renderCommandBuffer.WaitUntilCompleted();
	renderCommandBuffer.Release();
	// blit command buffer
	dispatch_semaphore_wait(m_semaphore, DISPATCH_TIME_FOREVER);
	mtlpp::CommandBuffer blitCommandBuffer = m_commandQueue.CommandBuffer();
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

    void* texture;
    void* drawable;
    this->getMetalHackBoth(&texture, &drawable);

	mtlpp::Texture blitTexture = mtlpp::Texture(ns::Handle{ texture });
	blitCommandEncoder.Copy(colorTexture, 0, 0, mtlpp::Origin(0, 0, 0), mtlpp::Size(800, 600, 1), blitTexture, 0, 0, mtlpp::Origin(0, 0, 0));
	blitCommandEncoder.EndEncoding();
	blitCommandEncoder.Release();
	mtlpp::Drawable blitDrawable = mtlpp::Drawable(ns::Handle{ drawable });
	blitCommandBuffer.Present(blitDrawable);
	blitCommandBuffer.Enqueue();
	blitCommandBuffer.Commit();
	blitCommandBuffer.WaitUntilCompleted();
	blitCommandBuffer.Release();
	colorTexture.Release();
	depthTexture.Release();

#else

    id<MTLDevice> device = (__bridge id<MTLDevice>)(m_device);
    id<MTLCommandQueue> commandQueue = (__bridge id<MTLCommandQueue>)(m_commandQueue);
    id<MTLBuffer> vertexBuffer = (__bridge id<MTLBuffer>)(m_vertexBuffer);
    id<MTLRenderPipelineState> renderPipelineState = (__bridge id<MTLRenderPipelineState>)(m_renderPipelineState);

    void* blitTexturePtr;
    void* blitDrawablePtr;
    this->getMetalHackBoth(&blitTexturePtr, &blitDrawablePtr);

    id<MTLTexture> blitTexture = CFBridgingRelease(blitTexturePtr);
    id<CAMetalDrawable> blitDrawable = CFBridgingRelease(blitDrawablePtr);

    // color
    MTLTextureDescriptor* colorTextureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatBGRA8Unorm_sRGB
                                                                                                      width:800
                                                                                                     height:600
                                                                                                  mipmapped:NO];
    colorTextureDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
    id<MTLTexture> colorTexture = [device newTextureWithDescriptor:colorTextureDescriptor];
    [colorTextureDescriptor release];

    // depth
    MTLTextureDescriptor* depthTextureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float_Stencil8
                                                                                                      width:800
                                                                                                     height:600
                                                                                                  mipmapped:NO];
    depthTextureDescriptor.storageMode = MTLStorageModePrivate;
    depthTextureDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
    id<MTLTexture> depthTexture = [device newTextureWithDescriptor:depthTextureDescriptor];
    [depthTextureDescriptor release];

    // draw render pass descriptor
    MTLRenderPassDescriptor* renderPassDescriptor = [MTLRenderPassDescriptor new];
    renderPassDescriptor.colorAttachments[0].texture = colorTexture;
    renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(rand(0.0f, 1.0f), rand(0.0f, 1.0f), rand(0.0f, 1.0f), rand(0.0f, 1.0f));
    [colorTexture release];
    renderPassDescriptor.depthAttachment.texture = depthTexture;
    renderPassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
    renderPassDescriptor.depthAttachment.storeAction = MTLStoreActionStore;
    renderPassDescriptor.depthAttachment.clearDepth = 1.0;
    renderPassDescriptor.stencilAttachment.texture = depthTexture;
    renderPassDescriptor.stencilAttachment.loadAction = MTLLoadActionClear;
    renderPassDescriptor.stencilAttachment.storeAction = MTLStoreActionStore;
    [depthTexture release];

    // draw command buffer
    id<MTLCommandBuffer> renderCommandBuffer = [commandQueue commandBuffer];
    [renderCommandBuffer addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull commandBuffer) {
        [commandBuffer release];
    }];
    id<MTLRenderCommandEncoder> renderCommandEncoder = [renderCommandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    [renderPassDescriptor release];
    [renderCommandEncoder setRenderPipelineState:renderPipelineState];
    [renderCommandEncoder setVertexBuffer:vertexBuffer offset:0 atIndex:0];
    [renderCommandEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
    [renderCommandEncoder endEncoding];
    [renderCommandBuffer enqueue];
    [renderCommandBuffer commit];

    dispatch_semaphore_wait(m_semaphore, DISPATCH_TIME_FOREVER);
    dispatch_semaphore_t blockSemaphore = m_semaphore;

    // blit command buffer
    id<MTLCommandBuffer> blitCommandBuffer = [commandQueue commandBuffer];
    [blitCommandBuffer addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull commandBuffer) {
#if 0
        std::cout << [[commandBuffer label] cStringUsingEncoding:NSUTF8StringEncoding] << std::endl;
#endif
        [commandBuffer release];
        // Add completion handler for this command buffer to advance frame
        // You must use this to change the captured class member variables
        dispatch_semaphore_signal(blockSemaphore);
     }];
    id<MTLBlitCommandEncoder> blitCommandEncoder = [blitCommandBuffer blitCommandEncoder];
    [blitCommandEncoder copyFromTexture:colorTexture
                            sourceSlice:0
                            sourceLevel:0
                           sourceOrigin:MTLOriginMake(0, 0, 0)
                             sourceSize:MTLSizeMake(800, 600, 1)
                              toTexture:blitTexture
                       destinationSlice:0
                       destinationLevel:0
                      destinationOrigin:MTLOriginMake(0, 0, 0)];
    [blitCommandEncoder endEncoding];
    [blitCommandEncoder release];
    [blitCommandBuffer presentDrawable:blitDrawable];
    [blitDrawable release];
    [blitCommandBuffer enqueue];
    [blitCommandBuffer commit];

#endif

#endif
}

void SampleWindow::onKeyDown(int k) {
}
