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

#ifndef VULKAN_SAMPLE_WINDOW_H_
#define VULKAN_SAMPLE_WINDOW_H_

#include <memory>
#include <string>

#if defined(_WIN32)
#include <Windows.h>
#include <string>
#else
#include <string.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#endif

/**
 * Encapsulate a window class.
 *
 * Calls <c>::RegisterClass()</c> on create, and <c>::UnregisterClass()</c>
 * on destruction.
 */
struct WindowClass final {
public:
#if defined(_WIN32)
	WindowClass(const std::string& name,
				::WNDPROC procedure = ::DefWindowProc);
#else
	WindowClass(const std::string& name);
#endif
	~WindowClass();

	const std::string& GetName() const;

	WindowClass(const WindowClass&) = delete;
	WindowClass& operator=(const WindowClass&) = delete;

private:
	std::string name_;
};

struct IWindow {
public:
	IWindow() = default;
	IWindow(const IWindow&) = delete;
	IWindow& operator=(const IWindow&) = delete;

	virtual ~IWindow();

	bool IsClosed() const;
	virtual void OnClose() = 0;

	int GetWidth() const;
	int GetHeight() const;

private:
	virtual bool IsClosedImpl() const = 0;
	virtual int GetWidthImpl() const = 0;
	virtual int GetHeightImpl() const = 0;
};

class Window : public IWindow {
public:
	Window(const std::string& title, int width, int height);
#if defined(_WIN32)
	HWND GetHWND() const;
	HINSTANCE GetHINSTANCE() const;
#else
	xcb_connection_t* GetConnection() const;
	xcb_window_t GetWindow() const;
#endif
	void OnClose() override {
		isClosed_ = true;
	}

private:
	bool IsClosedImpl() const override {
		return isClosed_;
	}

	int GetWidthImpl() const override {
		return width_;
	}

	int GetHeightImpl() const override {
		return height_;
	}

	std::unique_ptr<WindowClass> windowClass_;
	bool isClosed_ = false;
	int width_ = -1, height_ = -1;

#if defined(_WIN32)
	HWND hwnd_ = 0;
	HINSTANCE hinstance_ = 0;
#else
	xcb_connection_t* connection;
	xcb_screen_t* screen;
	xcb_window_t window;
	xcb_intern_atom_reply_t* atom_wm_delete_window;
#endif
};

#endif

#endif
