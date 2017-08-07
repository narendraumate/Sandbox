//
//  PuzzleWindow.h
//
//
//  Created by Narendra Umate on 8/18/15.
//
//

#pragma once

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

#include "VulkanSample.h"
// #include "VulkanQuad.h"
// #include "VulkanTexturedQuad.h"

class PuzzleWindow : public BaseWindow {
public:
	PuzzleWindow(Parameters& parameters);
	virtual ~PuzzleWindow();

	virtual void pollEvents(); // TODO Should remove this but crashes without it

	virtual void onKeyDown(int k);
	virtual void onResize(const int& xSize, const int& ySize);
	virtual void update(float dt);

private:
	void create();
	void destroy();

	Renderer* m_renderer;

#if defined(__linux__) || defined(_WIN32)
	VulkanSample* sample;
#endif
};
