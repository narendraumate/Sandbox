//
//  PuzzleWindow.cpp
//
//
//  Created by Narendra Umate on 8/18/15.
//
//

#include "PuzzleWindow.h"

#include "Platform.h"

#if defined(_WIN32)
#include "RendererDx.h" // needs to be after SampleWindow.h
#endif // defined(_WIN32)

#if defined(__APPLE__)
#include "RendererMl.h" // needs to be after SampleWindow.h
#endif // defined(__APPLE__)

#include "RendererGl.h" // needs to be after SampleWindow.h

#include "RendererVk.h" // needs to be after SampleWindow.h

PuzzleWindow::PuzzleWindow(Parameters& parameters)
: BaseWindow(parameters) {
	create();
}

PuzzleWindow::~PuzzleWindow() {
	destroy();
}

void PuzzleWindow::pollEvents() {
	// BaseWindow::pollEvents();
}

void PuzzleWindow::onResize(const int& xSize, const int& ySize) {
	BaseWindow::onResize(xSize, ySize);
}

void PuzzleWindow::onKeyDown(int k) {
}

void PuzzleWindow::update(float dt) {
	static int counter = 0;
#if defined(__linux__) || defined(_WIN32)
	sample->Run();
#endif
	if (counter++ == 300) {
		m_active = false;
	}
}

void PuzzleWindow::create() {
#if 0
	switch (m_type) {
		default:
		case None:
			break;
		case DirectX:
#if defined(_WIN32)
			m_renderer = new RendererDx(m_xSize, m_ySize);
#endif // defined(_WIN32)
			break;
		case Metal:
#if defined(__APPLE__)
			m_renderer = new RendererMl(m_xSize, m_ySize);
#endif // defined(__APPLE__)
			break;
		case OpenGL:
			m_renderer = new RendererGl(m_xSize, m_ySize);
			break;
		case Vulkan:
			m_renderer = new RendererVk(m_xSize, m_ySize);
			break;
	}
#endif
#if defined(__linux__) || defined(_WIN32)
	sample = new VulkanSample; // new VulkanQuad;
	sample->Before();
#endif
}

void PuzzleWindow::destroy() {
#if defined(__linux__) || defined(_WIN32)
	sample->After();
	delete sample;
#endif
}
