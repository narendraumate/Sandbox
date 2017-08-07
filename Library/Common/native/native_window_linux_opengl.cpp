//
//  native_window_linux_opengl.mm
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#if defined(__linux__)

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <dlfcn.h>

#include "native.h"
#include "native_linux_classes.h"
#include "native_window_linux_opengl.h"

NativeWindowLinuxOpenGL::NativeWindowLinuxOpenGL(const int& width, const int& height, const char* title)
: NativeWindowLinux(width, height, title) {
	// GL
	{
		const char* NAMES[] = { "libGL.so.1", "libGL.so" };
		unsigned int index = 0;
		for (index = 0; index < (sizeof(NAMES) / sizeof(NAMES[0])); ++index) {
			LibGL = dlopen(NAMES[index], RTLD_NOW | RTLD_GLOBAL);
			if (LibGL) {
				glXGetProcAddress = (PFNGLXGETPROCADDRESSPROC_PRIVATE)dlsym(LibGL, "glXGetProcAddressARB");
				glXCreateContext = (PFNGLXCREATECONTEXT_PRIVATE)dlsym(LibGL, "glXCreateContext");
				glXMakeCurrent = (PFNGLXMAKECURRENT_PRIVATE)dlsym(LibGL, "glXMakeCurrent");
				glXDestroyContext = (PFNGLXDESTROYCONTEXT_PRIVATE)dlsym(LibGL, "glXDestroyContext");
				glXSwapBuffers = (PFNGLXSWAPBUFFERS_PRIVATE)dlsym(LibGL, "glXSwapBuffers");
				glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXT_PRIVATE)dlsym(LibGL, "glXSwapIntervalEXT");
				break;
			}
		}
	}

	// X11
	{
		const char* NAMES[] = { "libX11.so.1", "libX11.so" };
		unsigned int index = 0;
		for (index = 0; index < (sizeof(NAMES) / sizeof(NAMES[0])); ++index) {
			LibX11 = dlopen(NAMES[index], RTLD_NOW | RTLD_GLOBAL);
			if (LibX11) {
				XOpenDisplay = (PFNXOPENDISPLAYPROC_PRIVATE)dlsym(LibX11, "XOpenDisplay");
				XCreateColormap = (PFNXCREATECOLORMAP_PRIVATE)dlsym(LibX11, "XCreateColormap");
				XStoreName = (PFNXSTORENAME_PRIVATE)dlsym(LibX11, "XStoreName");
				XCreateWindow = (PFNXCREATEWINDOW_PRIVATE)dlsym(LibX11, "XCreateWindow");
				XDestroyWindow = (PFNXDESTROYWINDOW_PRIVATE)dlsym(LibX11, "XDestroyWindow");
				XMapWindow = (PFNXMAPWINDOW_PRIVATE)dlsym(LibX11, "XMapWindow");
				XRaiseWindow = (PFNXRAISEWINDOW_PRIVATE)dlsym(LibX11, "XRaiseWindow");
				XInternAtom = (PFNXINTERNATOM_PRIVATE)dlsym(LibX11, "XInternAtom");
				XSetWMProtocols = (PFNXSETWMPROTOCOLS_PRIVATE)dlsym(LibX11, "XSetWMProtocols");
				XFree = (PFNXFREE_PRIVATE)dlsym(LibX11, "XFree");
				XUnmapWindow = (PFNXUNMAPWINDOW_PRIVATE)dlsym(LibX11, "XUnmapWindow");
				XCloseDisplay = (PFNXCLOSEDISPLAY_PRIVATE)dlsym(LibX11, "XCloseDisplay");
				break;
			}
		}
	}

	const int attrib[] = {
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_DOUBLEBUFFER, True,
		GLX_RED_SIZE, 1,
		GLX_GREEN_SIZE, 1,
		GLX_BLUE_SIZE, 1,
		GLX_SAMPLE_BUFFERS, 1,
		GLX_SAMPLES, 4,
		None
	};

	MainDisplay = XOpenDisplay(NULL);
	int screen = DefaultScreen(MainDisplay);
	Window root = RootWindow(MainDisplay, screen);

	int fbcount;
	PFNGLXCHOOSEFBCONFIGPROC glXChooseFBConfig = (PFNGLXCHOOSEFBCONFIGPROC)glXGetProcAddress((GLubyte*)"glXChooseFBConfig");
	GLXFBConfig* fbc = glXChooseFBConfig(MainDisplay, screen, attrib, &fbcount);

	PFNGLXGETVISUALFROMFBCONFIGPROC glXGetVisualFromFBConfig = (PFNGLXGETVISUALFROMFBCONFIGPROC)glXGetProcAddress((GLubyte*)"glXGetVisualFromFBConfig");
	XVisualInfo* visinfo = glXGetVisualFromFBConfig(MainDisplay, fbc[0]);

	XSetWindowAttributes attr;
	attr.background_pixel = 0;
	attr.border_pixel = 0;
	attr.colormap = XCreateColormap(MainDisplay, root, visinfo->visual, AllocNone);
	attr.backing_store = 0;
	attr.override_redirect = 0;
	attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;

	MainWindow = XCreateWindow(
		MainDisplay,
		root,
		0, 0,
		width, height, 0,
		visinfo->depth,
		InputOutput,
		visinfo->visual,
		CWBackPixel | CWBorderPixel | CWColormap | CWBackingStore | CWOverrideRedirect | CWEventMask,
		&attr);

	XMapWindow(MainDisplay, MainWindow);
	XStoreName(MainDisplay, MainWindow, title);
	XRaiseWindow(MainDisplay, MainWindow);

	Atom WM_DELETE_WINDOW = XInternAtom(MainDisplay, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(MainDisplay, MainWindow, &WM_DELETE_WINDOW, 1);

	if (true) {
		GLXContext tempContext = glXCreateContext(MainDisplay, visinfo, NULL, True);
		PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs = (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress((GLubyte*)"glXCreateContextAttribsARB");

		int fbcount = 0;
		GLXFBConfig* framebufferConfig = glXChooseFBConfig(MainDisplay, screen, 0, &fbcount);
		if (framebufferConfig) {
			const int attribs[] = {
				GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
				GLX_CONTEXT_MINOR_VERSION_ARB, 6,
				GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				0
			};
			glcontext = glXCreateContextAttribs(MainDisplay, framebufferConfig[0], NULL, True, attribs);
			glXMakeCurrent(MainDisplay, 0, 0);
			glXDestroyContext(MainDisplay, tempContext);
		}
	} else {
		glcontext = glXCreateContext(MainDisplay, visinfo, NULL, True);
	}
	XFree(visinfo);
}

NativeWindowLinuxOpenGL::~NativeWindowLinuxOpenGL() {
	glXDestroyContext(MainDisplay, glcontext);
	XUnmapWindow(MainDisplay, MainWindow);
	XDestroyWindow(MainDisplay, MainWindow);
	XCloseDisplay(MainDisplay);
}

void NativeWindowLinuxOpenGL::getWindowPos(int& xPos, int& yPos) {
	// TODO
}

void NativeWindowLinuxOpenGL::getWindowSize(int& width, int& height) {
	// TODO
}

void NativeWindowLinuxOpenGL::getFramebufferSize(int& width, int& height) {
	// TODO
	width = 800;
	height = 600;
}

void NativeWindowLinuxOpenGL::makeContextCurrent() {
	glXMakeCurrent(MainDisplay, MainWindow, glcontext);
}

void NativeWindowLinuxOpenGL::swapBuffers() {
	glXSwapBuffers(MainDisplay, MainWindow);
}

void NativeWindowLinuxOpenGL::swapInterval(const int& interval) {
	glXSwapIntervalEXT(MainDisplay, MainWindow, interval);
}

void* NativeWindowLinuxOpenGL::getProcAddress(const char* procname) {
	return (void*)glXGetProcAddress((const GLubyte*)procname);
}

#endif // defined(__linux__)
