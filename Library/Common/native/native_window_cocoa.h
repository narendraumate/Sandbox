//
//  native_window_cocoa.h
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#ifndef native_window_cocoa_h
#define native_window_cocoa_h

#if defined(__APPLE__)

#include "native_window_basic.h"

class NativeWindowCocoa : public NativeWindowBasic {
public:
	NativeWindowCocoa(const int& width, const int& height, const char* title);
	virtual ~NativeWindowCocoa();

	virtual void getFramebufferSize(int& width, int& height) = 0;
	virtual void makeContextCurrent() = 0;
	virtual void swapBuffers() = 0;
	virtual void swapInterval(const int& interval) = 0;
	virtual void* getProcAddress(const char* procname) = 0;

	virtual int getWidth() const;
	virtual int getHeight() const;

protected:
	void* m_cocoaWindow;
};

#endif // defined(__APPLE__)

#endif // native_window_cocoa_h
