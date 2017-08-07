//
//  native_window_cocoa_opengl.h
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#ifndef native_window_cocoa_opengl_h
#define native_window_cocoa_opengl_h

#include "native_window_cocoa.h"

class NativeWindowCocoaOpenGL : public NativeWindowCocoa {
public:
	NativeWindowCocoaOpenGL(const int& width, const int& height, const char* title);
	virtual ~NativeWindowCocoaOpenGL();

	virtual void getWindowPos(int& xPos, int& yPos);
	virtual void getWindowSize(int& width, int& height);
	virtual void getFramebufferSize(int& width, int& height);
	virtual void makeContextCurrent();
	virtual void swapBuffers();
	virtual void swapInterval(const int& interval);
	virtual void* getProcAddress(const char* procname);

	virtual void getMetalHackBoth(void** texture, void** drawable);

private:
	void* m_context;
};

#endif // native_window_cocoa_opengl_h
