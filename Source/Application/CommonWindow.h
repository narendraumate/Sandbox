//
//  CommonWindow.h
//
//
//  Created by Narendra Umate on 7/17/16.
//
//

#if defined(COMPILE_COMMON)
#pragma once

#include <unordered_map>

#include "AbstractWindow.h"
#include "glad/glad.h"

#if defined(__linux__) || defined(_WIN32)
#include "glfw3.h"
#include <vulkan/vulkan.h>
#else
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include "glfw3.h"
#include "glfw3native.h"
#endif
#include "glfw_config.h"

#define KEY_UP GLFW_KEY_UP
#define KEY_DOWN GLFW_KEY_DOWN
#define KEY_RIGHT GLFW_KEY_RIGHT
#define KEY_LEFT GLFW_KEY_LEFT
#define KEY_Q GLFW_KEY_Q
#define KEY_E GLFW_KEY_E

struct Parameters;

class CommonWindow : public AbstractWindow {

public:
	CommonWindow(Parameters& parameters);
	virtual ~CommonWindow() = 0;

	virtual void onResize(const int& xSize, const int& ySize);
	virtual void onFocus(const int& focus);

	virtual void update(float dt);
	virtual void swapBuffers();
	virtual void pollEvents();

	virtual void getWindowPos(int& xPos, int& yPos);
	virtual void getWindowSize(int& width, int& height);
	virtual void getFramebufferSize(int& width, int& height);

	virtual void getMetalHackBoth(void** texture, void** drawable);

	static std::unordered_map<GLFWwindow*, CommonWindow*> s_windowMap;

	// HACK Fix this so that it can be private again.
	GLFWwindow* m_glfwWindow;

private:
	void initializeCommon();
	void initializeMetal();
	void initializeOpenGL();
	void initializeVulkan();
	void deinitializeCommon();
	void deinitializeMetal();
	void deinitializeOpenGL();
	void deinitializeVulkan();

#if defined(__linux__) || defined(_WIN32)
	VkInstance instance;
#elif defined(__APPLE__)
	void* m_layer;
	void* m_drawable;
#endif // defined(__linux__) || defined(_WIN32)
};
#endif
