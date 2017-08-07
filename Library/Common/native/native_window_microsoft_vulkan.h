//
//  native_window_microsoft_vulkan.h
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#ifndef native_window_microsoft_vulkan_h
#define native_window_microsoft_vulkan_h

#if defined(_WIN32)

#include "native_window_microsoft.h"

#include <windows.h>

#include <GL/GL.h>

#pragma comment(lib, "opengl32.lib")

class NativeWindowMicrosoftVulkan : public NativeWindowMicrosoft {
public:
	NativeWindowMicrosoftVulkan(const int& width, const int& height, const char* title);
	virtual ~NativeWindowMicrosoftVulkan();

	virtual void getWindowPos(int& xPos, int& yPos);
	virtual void getWindowSize(int& width, int& height);
	virtual void getFramebufferSize(int& width, int& height);
	virtual void makeContextCurrent();
	virtual void swapBuffers();
	virtual void swapInterval(const int& interval);
	virtual void* getProcAddress(const char* procname);

private:
	void* m_context;
};

#endif // defined(_WIN32)

#endif // native_window_microsoft_vulkan_h
