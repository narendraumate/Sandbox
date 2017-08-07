//
//  native_window_microsoft_vulkan.cpp
//
//
//  Created by Narendra Umate on 11/19/16.
//
//

#if defined(_WIN32)

#include "native_microsoft_classes.h"

#include "native_window_microsoft_vulkan.h"

#include "windows.h"

// See https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt for all values
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

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

static HGLRC
init_vulkan(HDC real_dc) {
	HGLRC blah;
	return blah;
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
		"Vulkan",
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

NativeWindowMicrosoftVulkan::NativeWindowMicrosoftVulkan(const int& width, const int& height, const char* title)
: NativeWindowMicrosoft(width, height, title) {
	HWND window = create_window(width, height, title);
	HDC gldc = GetDC(window);
	HGLRC glrc = init_vulkan(gldc);
}

NativeWindowMicrosoftVulkan::~NativeWindowMicrosoftVulkan() {
	// TODO
}

void NativeWindowMicrosoftVulkan::getWindowPos(int& xPos, int& yPos) {
	// TODO
	xPos = 0;
	yPos = 0;
}

void NativeWindowMicrosoftVulkan::getWindowSize(int& width, int& height) {
	// TODO
	width = 800;
	height = 600;
}

void NativeWindowMicrosoftVulkan::getFramebufferSize(int& width, int& height) {
	// TODO
	width = 800;
	height = 600;
}

void NativeWindowMicrosoftVulkan::makeContextCurrent() {
	HDC pdc = wglGetCurrentDC();
	HGLRC prc = wglGetCurrentContext();
	wglMakeCurrent(pdc, prc);
}

void NativeWindowMicrosoftVulkan::swapBuffers() {
	SwapBuffers(wglGetCurrentDC());
}

void NativeWindowMicrosoftVulkan::swapInterval(const int& interval) {
	typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int);
	PFNWGLSWAPINTERVALEXTPROC s = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	s(interval);
}

void* NativeWindowMicrosoftVulkan::getProcAddress(const char* procname) {
	void* p = (void*)wglGetProcAddress(procname);
	if (!p) {
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void*)GetProcAddress(module, procname);
	}
	return p;
}

#endif // defined(_WIN32)
