//
//  SampleWindow.h
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
#include <unordered_map>

class SampleWindow : public BaseWindow {
public:
	SampleWindow(Parameters& parameters);
	virtual ~SampleWindow();

	virtual void onKeyDown(int k);
	virtual void onResize(const int& xSize, const int& ySize);
	virtual void update(float dt);

private:
	void create();
	void destroy();

	Renderer* m_renderer;
	SpatialCamera* m_camera;
	Bvh* m_bvh;
	Octree* m_octree;

	struct RenderPass {
		std::vector<Framebuffer*> m_framebufferVector;
	};

	std::vector<RenderPass*> m_renderPassVector;
	std::vector<SpatialLight*> m_spatialLightVector;
	std::vector<VisualEffect*> m_visualEffectVector;
};
