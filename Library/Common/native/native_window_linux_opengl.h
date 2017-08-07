//
//  native_window_linux_opengl.h
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#ifndef native_window_linux_opengl_h
#define native_window_linux_opengl_h

#if defined(__linux__)

#include "native_window_linux.h"

#include <GL/glx.h>

class NativeWindowLinuxOpenGL : public NativeWindowLinux {
public:
	NativeWindowLinuxOpenGL(const int& width, const int& height, const char* title);
	virtual ~NativeWindowLinuxOpenGL();

	virtual void getWindowPos(int& xPos, int& yPos);
	virtual void getWindowSize(int& width, int& height);
	virtual void getFramebufferSize(int& width, int& height);
	virtual void makeContextCurrent();
	virtual void swapBuffers();
	virtual void swapInterval(const int& interval);
	virtual void* getProcAddress(const char* procname);

private:
	// Members
	GLXContext glcontext;
	Display* MainDisplay;
	Window MainWindow;
	void* LibGL;
	void* LibX11;

	// GL functions
	typedef void*(APIENTRYP PFNGLXGETPROCADDRESSPROC_PRIVATE)(const GLubyte*);
	PFNGLXGETPROCADDRESSPROC_PRIVATE glXGetProcAddress;

	typedef GLXContext(APIENTRYP PFNGLXCREATECONTEXT_PRIVATE)(Display*, XVisualInfo*, GLXContext, Bool);
	PFNGLXCREATECONTEXT_PRIVATE glXCreateContext;

	typedef Bool(APIENTRYP PFNGLXMAKECURRENT_PRIVATE)(Display*, GLXDrawable, GLXContext);
	PFNGLXMAKECURRENT_PRIVATE glXMakeCurrent;

	typedef void(APIENTRYP PFNGLXDESTROYCONTEXT_PRIVATE)(Display*, GLXContext);
	PFNGLXDESTROYCONTEXT_PRIVATE glXDestroyContext;

	typedef void(APIENTRYP PFNGLXSWAPBUFFERS_PRIVATE)(Display*, GLXDrawable);
	PFNGLXSWAPBUFFERS_PRIVATE glXSwapBuffers;

	typedef void(APIENTRYP PFNGLXSWAPINTERVALEXT_PRIVATE)(Display*, GLXDrawable, int);
	PFNGLXSWAPINTERVALEXT_PRIVATE glXSwapIntervalEXT;

	// X11 functions
	typedef Display*(APIENTRYP PFNXOPENDISPLAYPROC_PRIVATE)(_Xconst char*);
	PFNXOPENDISPLAYPROC_PRIVATE XOpenDisplay;

	typedef Colormap(APIENTRYP PFNXCREATECOLORMAP_PRIVATE)(Display*, Window, Visual*, int);
	PFNXCREATECOLORMAP_PRIVATE XCreateColormap;

	typedef int(APIENTRYP PFNXSTORENAME_PRIVATE)(Display*, Window, _Xconst char*);
	PFNXSTORENAME_PRIVATE XStoreName;

	typedef Window(APIENTRYP PFNXCREATEWINDOW_PRIVATE)(Display*, Window, int, int, unsigned int, unsigned int, unsigned int, int, unsigned int, Visual*, unsigned long, XSetWindowAttributes*);
	PFNXCREATEWINDOW_PRIVATE XCreateWindow;

	typedef int(APIENTRYP PFNXDESTROYWINDOW_PRIVATE)(Display*, Window);
	PFNXDESTROYWINDOW_PRIVATE XDestroyWindow;

	typedef int(APIENTRYP PFNXMAPWINDOW_PRIVATE)(Display*, Window);
	PFNXMAPWINDOW_PRIVATE XMapWindow;

	typedef int(APIENTRYP PFNXRAISEWINDOW_PRIVATE)(Display*, Window);
	PFNXRAISEWINDOW_PRIVATE XRaiseWindow;

	typedef Atom(APIENTRYP PFNXINTERNATOM_PRIVATE)(Display*, _Xconst char*, Bool);
	PFNXINTERNATOM_PRIVATE XInternAtom;

	typedef Status(APIENTRYP PFNXSETWMPROTOCOLS_PRIVATE)(Display*, Window, Atom*, int);
	PFNXSETWMPROTOCOLS_PRIVATE XSetWMProtocols;

	typedef int(APIENTRYP PFNXFREE_PRIVATE)(void*);
	PFNXFREE_PRIVATE XFree;

	typedef int(APIENTRYP PFNXUNMAPWINDOW_PRIVATE)(Display*, Window);
	PFNXUNMAPWINDOW_PRIVATE XUnmapWindow;

	typedef int(APIENTRYP PFNXCLOSEDISPLAY_PRIVATE)(Display*);
	PFNXCLOSEDISPLAY_PRIVATE XCloseDisplay;
};

#endif // defined(__linux__)

#endif // native_window_linux_opengl_h
