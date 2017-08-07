//
//  native_window_linux_vulkan.cpp
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#if defined(__linux__)

#include "native_linux_classes.h"

#include "native_window_linux_vulkan.h"

NativeWindowLinuxVulkan::NativeWindowLinuxVulkan(const int& width, const int& height, const char* title)
: NativeWindowLinux(width, height, title) {
	// TODO
}

NativeWindowLinuxVulkan::~NativeWindowLinuxVulkan() {
	// TODO
}

void NativeWindowLinuxVulkan::getWindowPos(int& xPos, int& yPos) {
	// TODO
}

void NativeWindowLinuxVulkan::getWindowSize(int& width, int& height) {
	// TODO
}

void NativeWindowLinuxVulkan::getFramebufferSize(int& width, int& height) {
	// TODO
}

void NativeWindowLinuxVulkan::makeContextCurrent() {
	// TODO
}

void NativeWindowLinuxVulkan::swapBuffers() {
	// TODO
}

void NativeWindowLinuxVulkan::swapInterval(const int& interval) {
	// TODO
}

void* NativeWindowLinuxVulkan::getProcAddress(const char* procname) {
	return 0;
}

#endif // defined(__linux__)
