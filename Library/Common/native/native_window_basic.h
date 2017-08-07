//
//  native_window_basic.h
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#ifndef native_window_basic_h
#define native_window_basic_h

#include "native.h"

class NativeWindowBasic {
public:
	NativeWindowBasic(const int& width, const int& height, const char* title);
	virtual ~NativeWindowBasic();

	virtual void getWindowPos(int& xPos, int& yPos) = 0;
	virtual void getWindowSize(int& width, int& height) = 0;
	virtual void getFramebufferSize(int& width, int& height) = 0;
	virtual void makeContextCurrent() = 0;
	virtual void swapBuffers() = 0;
	virtual void swapInterval(const int& interval) = 0;
	virtual void* getProcAddress(const char* procname) = 0;

	virtual void getMetalHackBoth(void** texture, void** drawable) = 0;

	struct {
		Nativewindowposfun pos;
		Nativewindowsizefun size;
		Nativewindowclosefun close;
		Nativewindowrefreshfun refresh;
		Nativewindowfocusfun focus;
		Nativewindowiconifyfun iconify;
		Nativewindowmaximizefun maximize;
		Nativeframebuffersizefun fbsize;
		// Nativemousebuttonfun mouseButton;
		// Nativecursorposfun cursorPos;
		// Nativecursorenterfun cursorEnter;
		// Nativescrollfun scroll;
		Nativekeyfun key;
		// Nativecharfun character;
		// Nativecharmodsfun charmods;
		// Nativedropfun drop;
	} callbacks;
};

#endif // native_window_basic_h
