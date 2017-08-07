//
//  NativeWindow.h
//
//
//  Created by Narendra Umate on 7/17/16.
//
//

#if defined(COMPILE_NATIVE)
#pragma once

#include <unordered_map>

#include "AbstractWindow.h"
#include "glad/glad.h"

#if defined(__linux__) || defined(_WIN32)
#include <vulkan/vulkan.h>
#endif // defined(__linux__) || defined(_WIN32)
#include "native.h"

#define KEY_UP NATIVE_KEY_UP
#define KEY_DOWN NATIVE_KEY_DOWN
#define KEY_RIGHT NATIVE_KEY_RIGHT
#define KEY_LEFT NATIVE_KEY_LEFT
#define KEY_Q NATIVE_KEY_Q
#define KEY_E NATIVE_KEY_E

struct Parameters;

class NativeWindow : public AbstractWindow {

public:
	NativeWindow(Parameters& parameters);
	virtual ~NativeWindow() = 0;

	virtual void onResize(const int& xSize, const int& ySize);
	virtual void onFocus(const int& focus);

	virtual void update(float dt);
	virtual void swapBuffers();
	virtual void pollEvents();

	virtual void getWindowPos(int& xPos, int& yPos);
	virtual void getWindowSize(int& width, int& height);
	virtual void getFramebufferSize(int& width, int& height);

	virtual void getMetalHackBoth(void** texture, void** drawable);

	static std::unordered_map<NativeWindowBasic*, NativeWindow*> s_windowMap;

	// HACK Fix this so that it can be private again.
	NativeWindowBasic* m_nativeWindow;

private:
	void initializeNative();
	void initializeDirectX();
	void initializeMetal();
	void initializeOpenGL();
	void initializeVulkan();
	void deinitializeNative();
	void deinitializeDirectX();
	void deinitializeMetal();
	void deinitializeOpenGL();
	void deinitializeVulkan();

#if defined(__linux__) || defined(_WIN32)
	VkInstance instance;
#elif defined(__APPLE__)
	void* layer;
	void* drawable;
#endif // defined(__linux__) || defined(_WIN32)
};
#endif
