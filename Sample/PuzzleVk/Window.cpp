//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#if defined(__linux__) || defined(_WIN32)

#include "Window.h"

namespace {
///////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
LRESULT CALLBACK aWndProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) {
	auto ptr = ::GetWindowLongPtr(hwnd, GWLP_USERDATA);
	auto window = reinterpret_cast<IWindow*>(ptr);

	switch (uMsg) {
		case WM_CLOSE:
			window->OnClose();
			return 0;
	}

	return ::DefWindowProcA(hwnd, uMsg, wParam, lParam);
}
#else
xcb_intern_atom_reply_t* intern_atom_helper(xcb_connection_t* conn, bool only_if_exists, const char* str) {
	xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, only_if_exists, strlen(str), str);
	return xcb_intern_atom_reply(conn, cookie, NULL);
}
#endif
} // namespace

///////////////////////////////////////////////////////////////////////////////
IWindow::~IWindow() {
}

///////////////////////////////////////////////////////////////////////////////
int IWindow::GetWidth() const {
	return GetWidthImpl();
}

///////////////////////////////////////////////////////////////////////////////
int IWindow::GetHeight() const {
	return GetHeightImpl();
}

///////////////////////////////////////////////////////////////////////////////
Window::Window(const std::string& title, const int width, const int height)
: width_(width)
, height_(height) {
#if defined(_WIN32)
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

	::RECT rect;
	::SetRect(&rect, 0, 0, width, height);
	::AdjustWindowRect(&rect, style, FALSE);

	windowClass_.reset(new WindowClass(/*"VulkanSampleWindowClass"*/ title, aWndProc));

	// Create the main window.
	hwnd_ = ::CreateWindowA(windowClass_->GetName().c_str(),
							title.c_str(),
							style, CW_USEDEFAULT, CW_USEDEFAULT,
							rect.right - rect.left, rect.bottom - rect.top, (HWND)NULL,
							(HMENU)NULL, NULL, (LPVOID)NULL);

	hinstance_ = ::GetModuleHandle(nullptr);

	::SetWindowLongPtr(hwnd_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	// Show the window and paint its contents.
	::ShowWindow(hwnd_, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd_);
#else
	const xcb_setup_t* setup;
	xcb_screen_iterator_t iter;
	int scr;

	connection = xcb_connect(NULL, &scr);
	if (connection == NULL) {
		printf("Could not find a compatible Vulkan ICD!\n");
		fflush(stdout);
		exit(1);
	}

	setup = xcb_get_setup(connection);
	iter = xcb_setup_roots_iterator(setup);
	while (scr-- > 0)
		xcb_screen_next(&iter);
	screen = iter.data;

	uint32_t value_mask, value_list[32];

	window = xcb_generate_id(connection);

	value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	value_list[0] = screen->black_pixel;
	value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE;

	/*if (settings.fullscreen)
	{
		width = destWidth = screen->width_in_pixels;
		height = destHeight = screen->height_in_pixels;
	}*/

	xcb_create_window(connection,
					  XCB_COPY_FROM_PARENT,
					  window, screen->root,
					  0, 0, width, height, 0,
					  XCB_WINDOW_CLASS_INPUT_OUTPUT,
					  screen->root_visual,
					  value_mask, value_list);

	/* Magic code that will send notification when window is destroyed */
	xcb_intern_atom_reply_t* reply = intern_atom_helper(connection, true, "WM_PROTOCOLS");
	atom_wm_delete_window = intern_atom_helper(connection, false, "WM_DELETE_WINDOW");

	xcb_change_property(connection, XCB_PROP_MODE_REPLACE,
						window, (*reply).atom, 4, 32, 1,
						&(*atom_wm_delete_window).atom);

	std::string windowTitle = /*getWindowTitle()*/ title;
	xcb_change_property(connection, XCB_PROP_MODE_REPLACE,
						window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
						windowTitle.size(), windowTitle.c_str());

	free(reply);

	/*if (settings.fullscreen)
	{
		xcb_intern_atom_reply_t *atom_wm_state = intern_atom_helper(connection, false, "_NET_WM_STATE");
		xcb_intern_atom_reply_t *atom_wm_fullscreen = intern_atom_helper(connection, false, "_NET_WM_STATE_FULLSCREEN");
		xcb_change_property(connection,
				XCB_PROP_MODE_REPLACE,
				window, atom_wm_state->atom,
				XCB_ATOM_ATOM, 32, 1,
				&(atom_wm_fullscreen->atom));
		free(atom_wm_fullscreen);
		free(atom_wm_state);
	}*/

	xcb_map_window(connection, window);
#endif
}

///////////////////////////////////////////////////////////////////////////////
bool IWindow::IsClosed() const {
	return IsClosedImpl();
}

///////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
HWND Window::GetHWND() const {
	return hwnd_;
}

HINSTANCE Window::GetHINSTANCE() const {
	return hinstance_;
}
#else
xcb_connection_t* Window::GetConnection() const {
	return connection;
}

xcb_window_t Window::GetWindow() const {
	return window;
}
#endif

/////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
WindowClass::WindowClass(const std::string& name, ::WNDPROC procedure)
: name_(name) {
	::WNDCLASSA wc;

	// Register the window class for the main window.
	wc.style = 0;
	wc.lpfnWndProc = procedure;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = name_.c_str();

	::RegisterClassA(&wc);
}
#else
WindowClass::WindowClass(const std::string& name)
: name_(name) {
}
#endif

/////////////////////////////////////////////////////////////////////////
const std::string& WindowClass::GetName() const {
	return name_;
}

/////////////////////////////////////////////////////////////////////////
WindowClass::~WindowClass() {
#if defined(_WIN32)
	::UnregisterClassA(name_.c_str(),
					   (HINSTANCE)::GetModuleHandle(NULL));
#else
#endif
}

#endif
