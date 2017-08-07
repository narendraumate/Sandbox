//
//  native.cpp
//
//
//  Created by Narendra Umate on 7/17/16.
//
//

#include "native.h"

#include "native_window_basic.h"

#if defined(__APPLE__)
#include <stdio.h> // sprintf
#endif

#if defined(_WIN32)
#include "native_microsoft_methods.h"
#include "native_window_microsoft_opengl.h"
#include "native_window_microsoft_vulkan.h"
#endif // defined(_WIN32)

#if defined(__APPLE__)
#include "native_cocoa_methods.h"
#include "native_window_cocoa_metal.h"
#include "native_window_cocoa_opengl.h"
#endif // defined(__APPLE__)

#if defined(__linux__)
#include "native_linux_methods.h"
#include "native_window_linux_opengl.h"
#include "native_window_linux_vulkan.h"
#endif // defined(__linux__)

// Swap the provided pointers of a give type.
#define _NATIVE_SWAP_POINTERS(type, x, y) \
	{                                     \
		type v;                           \
		v = x;                            \
		x = y;                            \
		y = v;                            \
	}

NativeWindowBasic* nativeWindowBasic = 0;

Nativeerrorfun err = 0;

int nativeContextVersionMajor = 0; // HACK

int nativeContextVersionMinor = 0; // HACK

Nativeerrorfun nativeSetErrorCallback(Nativeerrorfun cbfun) {
	_NATIVE_SWAP_POINTERS(Nativeerrorfun, err, cbfun);
	return cbfun;
}

Nativewindowposfun nativeSetWindowPosCallback(NativeWindowBasic* window, Nativewindowposfun cbfun) {
	if (window) {
		_NATIVE_SWAP_POINTERS(Nativewindowposfun, window->callbacks.pos, cbfun);
	}
	return cbfun;
}

Nativewindowsizefun nativeSetWindowSizeCallback(NativeWindowBasic* window, Nativewindowsizefun cbfun) {
	if (window) {
		_NATIVE_SWAP_POINTERS(Nativewindowsizefun, window->callbacks.size, cbfun);
	}
	return cbfun;
}

Nativewindowclosefun nativeSetWindowCloseCallback(NativeWindowBasic* window, Nativewindowclosefun cbfun) {
	if (window) {
		_NATIVE_SWAP_POINTERS(Nativewindowclosefun, window->callbacks.close, cbfun);
	}
	return cbfun;
}

Nativewindowrefreshfun nativeSetWindowRefreshCallback(NativeWindowBasic* window, Nativewindowrefreshfun cbfun) {
	if (window) {
		_NATIVE_SWAP_POINTERS(Nativewindowrefreshfun, window->callbacks.refresh, cbfun);
	}
	return cbfun;
}

Nativewindowfocusfun nativeSetWindowFocusCallback(NativeWindowBasic* window, Nativewindowfocusfun cbfun) {
	if (window) {
		_NATIVE_SWAP_POINTERS(Nativewindowfocusfun, window->callbacks.focus, cbfun);
	}
	return cbfun;
}

Nativewindowiconifyfun nativeSetWindowIconifyCallback(NativeWindowBasic* window, Nativewindowiconifyfun cbfun) {
	if (window) {
		_NATIVE_SWAP_POINTERS(Nativewindowiconifyfun, window->callbacks.iconify, cbfun);
	}
	return cbfun;
}

Nativewindowmaximizefun nativeSetWindowMaximizeCallback(NativeWindowBasic* window, Nativewindowmaximizefun cbfun) {
	if (window) {
		_NATIVE_SWAP_POINTERS(Nativewindowmaximizefun, window->callbacks.maximize, cbfun);
	}
	return cbfun;
}

Nativeframebuffersizefun nativeSetFramebufferSizeCallback(NativeWindowBasic* window, Nativeframebuffersizefun cbfun) {
	if (window) {
		_NATIVE_SWAP_POINTERS(Nativeframebuffersizefun, window->callbacks.fbsize, cbfun);
	}
	return cbfun;
}

Nativekeyfun nativeSetKeyCallback(NativeWindowBasic* window, Nativekeyfun cbfun) {
	if (window) {
		_NATIVE_SWAP_POINTERS(Nativekeyfun, window->callbacks.key, cbfun);
	}
	return cbfun;
}

void nativeWindowHint(int hint, int value) {
	if (hint == NATIVE_CONTEXT_VERSION_MAJOR) {
		nativeContextVersionMajor = value;
	} else if (hint == NATIVE_CONTEXT_VERSION_MINOR) {
		nativeContextVersionMinor = value;
	}
}

int nativeInit(void) {
	return platformInit();
}

void nativePollEvents(void) {
	platformPollEvents();
}

void nativeTerminate(void) {
	platformTerminate();
}

NativeWindowBasic* nativeCreateWindowDirectX(int width, int height, const char* title, NativeMonitorBasic* monitor, NativeWindowBasic* share) {
	NativeWindowBasic* window = 0;
	// TODO Implement this.
	return window;
}

NativeWindowBasic* nativeCreateWindowMetal(int width, int height, const char* title, NativeMonitorBasic* monitor, NativeWindowBasic* share) {
	NativeWindowBasic* window = 0;

#if defined(__APPLE__)
	window = new NativeWindowCocoaMetal(width, height, title);
#endif // defined(__APPLE__)

	return window;
}

NativeWindowBasic* nativeCreateWindowOpenGL(int width, int height, const char* title, NativeMonitorBasic* monitor, NativeWindowBasic* share) {
	NativeWindowBasic* window = 0;

#if defined(_WIN32)
	window = new NativeWindowMicrosoftOpenGL(width, height, title);
#endif // defined(_WIN32)

#if defined(__APPLE__)
	if (nativeContextVersionMajor == 4 && nativeContextVersionMinor == 1) {
		window = new NativeWindowCocoaOpenGL(width, height, title);
	} else {
		char buf[64];
		sprintf(buf, "Requested OpenGL version %d.%d, got version 4.1", nativeContextVersionMajor, nativeContextVersionMinor);
		err(65543, buf);
	}
#endif // defined(__APPLE__)

#if defined(__linux__)
	window = new NativeWindowLinuxOpenGL(width, height, title);
#endif // defined(__linux__)

	return window;
}

NativeWindowBasic* nativeCreateWindowVulkan(int width, int height, const char* title, NativeMonitorBasic* monitor, NativeWindowBasic* share) {
	NativeWindowBasic* window = 0;

#if defined(_WIN32)
	window = new NativeWindowMicrosoftVulkan(width, height, title);
#endif // defined(_WIN32)

#if defined(__APPLE__)
#endif // defined(__APPLE__)

#if defined(__linux__)
	window = new NativeWindowLinuxVulkan(width, height, title);
#endif // defined(__linux__)

	return window;
}

void nativeDestroyWindow(NativeWindowBasic* window) {
	if (window) {
		delete window;
		window = 0;
	}
}

void nativeGetWindowPos(NativeWindowBasic* window, int* xPos, int* yPos) {
	window->getWindowPos(*xPos, *yPos);
}

void nativeGetWindowSize(NativeWindowBasic* window, int* width, int* height) {
	window->getWindowSize(*width, *height);
}

void nativeGetFramebufferSize(NativeWindowBasic* window, int* width, int* height) {
	window->getFramebufferSize(*width, *height);
}

void nativeMakeContextCurrent(NativeWindowBasic* window) {
	window->makeContextCurrent();
	nativeWindowBasic = window;
}

void nativeSwapBuffers(NativeWindowBasic* window) {
	window->swapBuffers();
}

void nativeSwapInterval(int interval) {
	nativeWindowBasic->swapInterval(interval);
}

void* nativeGetProcAddress(const char* procname) {
	return nativeWindowBasic->getProcAddress(procname);
}

void nativeGetMetalHackBoth(void** texture, void** drawable) {
	nativeWindowBasic->getMetalHackBoth(texture, drawable);
}
