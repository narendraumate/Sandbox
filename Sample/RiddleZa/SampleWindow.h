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

#include "ECS.h"
#include "System/PhysicsSystem.h"
#include "System/RenderSystem.h"

using namespace ECS;

class SampleWindow : public BaseWindow {
public:
	SampleWindow(Parameters& parameters);
	virtual ~SampleWindow();

	virtual void onFocus(const int& focus);
	virtual void onKeyDown(int k);
	virtual void onResize(const int& xSize, const int& ySize);
	virtual void update(float dt);

private:
	World* world;
	PhysicsSystem* physicsSystem;
	RenderSystem* renderSystem;
	std::vector<Entity*> entityVector;
};
