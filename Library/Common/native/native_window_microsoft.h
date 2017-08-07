//
//  native_window_microsoft.h
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#ifndef native_window_microsoft_h
#define native_window_microsoft_h

#include "native_window_basic.h"

class NativeWindowMicrosoft : public NativeWindowBasic {
public:
	NativeWindowMicrosoft(const int& width, const int& height, const char* title);
	virtual ~NativeWindowMicrosoft();

	virtual void getFramebufferSize(int& width, int& height) = 0;
	virtual void makeContextCurrent() = 0;
	virtual void swapBuffers() = 0;
	virtual void swapInterval(const int& interval) = 0;
	virtual void* getProcAddress(const char* procname) = 0;

	virtual void getMetalHackBoth(void** texture, void** drawable);

protected:
	void* m_microsoftWindow;
};

#endif // native_window_microsoft_h
