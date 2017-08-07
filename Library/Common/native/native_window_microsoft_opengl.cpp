//
//  native_window_microsoft_opengl.mm
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#if defined(_WIN32)

#include "native_microsoft_classes.h"

#include "native_window_microsoft_opengl.h"

#include "windows.h"

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int* attribList);
wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;

// See https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt for all values
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;

// See https://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt for all values
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023

#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_TYPE_RGBA_ARB 0x202B

static void
fatal_error(char* msg) {
	MessageBoxA(NULL, msg, "Error", MB_OK | MB_ICONEXCLAMATION);
	exit(EXIT_FAILURE);
}

static void
init_opengl_extensions(void) {
	// Before we can load extensions, we need a dummy OpenGL context, created using a dummy window.
	// We use a dummy window because you can only set the pixel format for a window once. For the
	// real window, we want to use wglChoosePixelFormatARB (so we can potentially specify options
	// that aren't available in PIXELFORMATDESCRIPTOR), but we can't load and use that before we
	// have a context.

	WNDCLASSA window_class = { 0 };
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = DefWindowProcA;
	window_class.hInstance = GetModuleHandle(0);
	window_class.hCursor = LoadCursor(0, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	window_class.lpszClassName = "Dummy";

	if (!RegisterClassA(&window_class)) {
		fatal_error("Failed to register dummy OpenGL window.");
	}

	HWND dummy_window = CreateWindowExA(
		0,
		window_class.lpszClassName,
		"Dummy",
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		window_class.hInstance,
		0);

	if (!dummy_window) {
		fatal_error("Failed to create dummy OpenGL window.");
	}

	HDC dummy_dc = GetDC(dummy_window);

	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
	if (!pixel_format) {
		fatal_error("Failed to find a suitable pixel format.");
	}
	if (!SetPixelFormat(dummy_dc, pixel_format, &pfd)) {
		fatal_error("Failed to set the pixel format.");
	}

	HGLRC dummy_context = wglCreateContext(dummy_dc);
	if (!dummy_context) {
		fatal_error("Failed to create a dummy OpenGL rendering context.");
	}

	if (!wglMakeCurrent(dummy_dc, dummy_context)) {
		fatal_error("Failed to activate dummy OpenGL rendering context.");
	}

	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");

	wglMakeCurrent(dummy_dc, 0);
	wglDeleteContext(dummy_context);
	ReleaseDC(dummy_window, dummy_dc);
	DestroyWindow(dummy_window);
}

static HGLRC
init_opengl(HDC real_dc) {
	init_opengl_extensions();

	// Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
	int pixel_format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0
	};

	int pixel_format;
	UINT num_formats;
	wglChoosePixelFormatARB(real_dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
	if (!num_formats) {
		fatal_error("Failed to set the OpenGL 4.1 pixel format.");
	}

	PIXELFORMATDESCRIPTOR pfd;
	DescribePixelFormat(real_dc, pixel_format, sizeof(pfd), &pfd);
	if (!SetPixelFormat(real_dc, pixel_format, &pfd)) {
		fatal_error("Failed to set the OpenGL 4.1 pixel format.");
	}

	// Specify that we want to create an OpenGL 4.1 core profile context
	int gl41_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB,
		4,
		WGL_CONTEXT_MINOR_VERSION_ARB,
		1,
		WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0,
	};

	HGLRC gl41_context = wglCreateContextAttribsARB(real_dc, 0, gl41_attribs);
	if (!gl41_context) {
		fatal_error("Failed to create OpenGL 4.1 context.");
	}

	if (!wglMakeCurrent(real_dc, gl41_context)) {
		fatal_error("Failed to activate OpenGL 4.1 rendering context.");
	}

	return gl41_context;
}

static LRESULT CALLBACK
window_callback(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
	LRESULT result = 0;

	switch (msg) {
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			result = DefWindowProcA(window, msg, wparam, lparam);
			break;
	}

	return result;
}

static HWND
create_window(const int& width, const int& height, const char* title) {
	HINSTANCE hInstance = GetModuleHandleW(0);
	WNDCLASSA window_class = { 0 };
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = window_callback;
	window_class.hInstance = hInstance;
	window_class.hCursor = LoadCursor(0, IDC_ARROW);
	window_class.hbrBackground = 0;
	window_class.lpszClassName = "oglversionchecksample";

	if (!RegisterClassA(&window_class)) {
		fatal_error("Failed to register window.");
	}

	// Specify a desired width and height, then adjust the rect so the window's client area will be
	// that size.
	RECT rect = { 0 };
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	DWORD window_style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&rect, window_style, false);

	HWND window = CreateWindowExA(
		0,
		window_class.lpszClassName,
		"OpenGL",
		window_style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		0,
		0,
		hInstance,
		0);

	if (!window) {
		fatal_error("Failed to create window.");
	}

	return window;
}

NativeWindowMicrosoftOpenGL::NativeWindowMicrosoftOpenGL(const int& width, const int& height, const char* title)
: NativeWindowMicrosoft(width, height, title) {
	HWND window = create_window(width, height, title);
	HDC gldc = GetDC(window);
	HGLRC glrc = init_opengl(gldc);
}

NativeWindowMicrosoftOpenGL::~NativeWindowMicrosoftOpenGL() {
	// TODO
}

void NativeWindowMicrosoftOpenGL::getWindowPos(int& xPos, int& yPos) {
	// TODO
	xPos = 0;
	yPos = 0;
}

void NativeWindowMicrosoftOpenGL::getWindowSize(int& width, int& height) {
	// TODO
	width = 800;
	height = 600;
}

void NativeWindowMicrosoftOpenGL::getFramebufferSize(int& width, int& height) {
	// TODO
	width = 800;
	height = 600;
}

void NativeWindowMicrosoftOpenGL::makeContextCurrent() {
	HDC pdc = wglGetCurrentDC();
	HGLRC prc = wglGetCurrentContext();
	wglMakeCurrent(pdc, prc);
}

void NativeWindowMicrosoftOpenGL::swapBuffers() {
	SwapBuffers(wglGetCurrentDC());
}

void NativeWindowMicrosoftOpenGL::swapInterval(const int& interval) {
	typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int);
	PFNWGLSWAPINTERVALEXTPROC s = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	s(interval);
}

void* NativeWindowMicrosoftOpenGL::getProcAddress(const char* procname) {
	void* p = (void*)wglGetProcAddress(procname);
	if (!p) {
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void*)GetProcAddress(module, procname);
	}
	return p;
}

#endif // defined(_WIN32)
