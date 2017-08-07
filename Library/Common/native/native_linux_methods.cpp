//
//  native_linux_methods.cpp
//
//
//  Created by Narendra Umate on 7/17/16.
//
//

#if defined(__linux__)

#include "native_linux_methods.h"
#include "native_window_linux_opengl.h"
#include "native_window_linux_vulkan.h"

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <dlfcn.h>

extern NativeWindowBasic* nativeWindowBasic;

int platformInit() {
	return 1;
}

void platformPollEvents() {
	// GL
	void* LibGL;
	typedef Display*(APIENTRYP PFNGLXGETCURRENTDISPLAY_PRIVATE)(void);
	PFNGLXGETCURRENTDISPLAY_PRIVATE glXGetCurrentDisplay;
	{
		const char* NAMES[] = { "libGL.so.1", "libGL.so" };
		unsigned int index = 0;
		for (index = 0; index < (sizeof(NAMES) / sizeof(NAMES[0])); ++index) {
			LibGL = dlopen(NAMES[index], RTLD_NOW | RTLD_GLOBAL);
			if (LibGL) {
				glXGetCurrentDisplay = (PFNGLXGETCURRENTDISPLAY_PRIVATE)dlsym(LibGL, "glXGetCurrentDisplay");
				break;
			}
		}
	}

	// X11
	void* LibX11;
	typedef int(APIENTRYP PFNXPENDING_PRIVATE)(Display*);
	PFNXPENDING_PRIVATE XPending;
	typedef int(APIENTRYP PFNXNEXTEVENT_PRIVATE)(Display*, XEvent*);
	PFNXNEXTEVENT_PRIVATE XNextEvent;
	typedef int(APIENTRYP PFNXFLUSH_PRIVATE)(Display*);
	PFNXFLUSH_PRIVATE XFlush;
	{
		const char* NAMES[] = { "libX11.so.1", "libX11.so" };
		unsigned int index = 0;
		for (index = 0; index < (sizeof(NAMES) / sizeof(NAMES[0])); ++index) {
			LibX11 = dlopen(NAMES[index], RTLD_NOW | RTLD_GLOBAL);
			if (LibX11) {
				XPending = (PFNXPENDING_PRIVATE)dlsym(LibX11, "XPending");
				XNextEvent = (PFNXNEXTEVENT_PRIVATE)dlsym(LibX11, "XNextEvent");
				XFlush = (PFNXFLUSH_PRIVATE)dlsym(LibX11, "XFlush");
				break;
			}
		}
	}

	Display* display = glXGetCurrentDisplay();
	int count = XPending(display);
	while (count--) {
		XEvent event;
		XNextEvent(display, &event);
		switch (event.type) {
			case KeyPress:
				if (nativeWindowBasic->callbacks.key) {
					// TODO Convert xkey.keycode to native keycode. Currently handling escape as special case.
					int nativeKeycode = event.xkey.keycode == 9 ? 256 : 0;
					nativeWindowBasic->callbacks.key((NativeWindowBasic*)nativeWindowBasic, nativeKeycode, event.xkey.keycode, NATIVE_PRESS, 0);
				}
				break;

			case KeyRelease:
				if (nativeWindowBasic->callbacks.key) {
					// TODO Convert xkey.keycode to native keycode. Currently handling escape as special case.
					int nativeKeycode = event.xkey.keycode == 9 ? 256 : 0;
					nativeWindowBasic->callbacks.key((NativeWindowBasic*)nativeWindowBasic, nativeKeycode, event.xkey.keycode, NATIVE_RELEASE, 0);
				}
				break;

			case ButtonPress:
				break;

			case ButtonRelease:
				break;

			case CreateNotify:
				break;

			case DestroyNotify:
				break;

			case ClientMessage:
				if (nativeWindowBasic->callbacks.close) {
					nativeWindowBasic->callbacks.close((NativeWindowBasic*)nativeWindowBasic);
				}
				break;

			default:
				break;
		}
	}
	XFlush(display);
}

void platformTerminate() {
}

#endif // defined(__linux__)
