//
//  native_window_linux.h
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#ifndef native_window_linux_h
#define native_window_linux_h

#if defined(__linux__)

#include "native_window_basic.h"

class NativeWindowLinux : public NativeWindowBasic {
public:
	NativeWindowLinux(const int& width, const int& height, const char* title);
	virtual ~NativeWindowLinux();

	virtual void getFramebufferSize(int& width, int& height) = 0;
	virtual void makeContextCurrent() = 0;
	virtual void swapBuffers() = 0;
	virtual void swapInterval(const int& interval) = 0;
	virtual void* getProcAddress(const char* procname) = 0;

	virtual void getMetalHackBoth(void** texture, void** drawable);
};

#endif // defined(__linux__)

#endif // native_window_linux_h
