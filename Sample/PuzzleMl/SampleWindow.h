//
//  SampleWindow.h
//
//
//  Created by Narendra Umate on 8/18/15.
//
//

#pragma once

#define USE_MTLPP 0

#if defined(COMPILE_COMMON)
#define BaseWindow CommonWindow
#include "CommonWindow.h"
#elif defined(COMPILE_NATIVE)
#define BaseWindow NativeWindow
#include "NativeWindow.h"
#endif

#include "Mesh.h"
#include "SpatialBoundVisual.h"
#include "SpatialLight.h"
#include <list>

#if defined(__APPLE__)
#if USE_MTLPP
#include "mtlpp.hpp"
#endif
#include <Dispatch/Dispatch.h>
#endif

class SampleWindow : public BaseWindow {
public:
	SampleWindow(Parameters& parameters);
	virtual ~SampleWindow();

	virtual void onFocus(const int& focus);
	virtual void onKeyDown(int k);
	virtual void onResize(const int& xSize, const int& ySize);
	virtual void update(float dt);

#if defined(__APPLE__)
private:
#if USE_MTLPP
	mtlpp::Device m_device;
	mtlpp::CommandQueue m_commandQueue;
	mtlpp::Buffer m_vertexBuffer;
	mtlpp::RenderPipelineState m_renderPipelineState;
#else
	void* m_device;
	void* m_commandQueue;
	void* m_vertexBuffer;
	void* m_renderPipelineState;
#endif
	dispatch_semaphore_t m_semaphore;
#endif
};
