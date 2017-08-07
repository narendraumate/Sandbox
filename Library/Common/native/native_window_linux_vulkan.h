//
//  native_window_linux_vulkan.h
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#ifndef native_window_linux_vulkan_h
#define native_window_linux_vulkan_h

#if defined(__linux__)

#include "native_window_linux.h"

class NativeWindowLinuxVulkan : public NativeWindowLinux {
public:
	NativeWindowLinuxVulkan(const int& width, const int& height, const char* title);
	virtual ~NativeWindowLinuxVulkan();

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

#endif // defined(__linux__)

#endif // native_window_linux_vulkan_h
