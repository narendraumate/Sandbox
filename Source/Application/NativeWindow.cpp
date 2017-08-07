//
//  NativeWindow.cpp
//
//
//  Created by Narendra Umate on 8/17/15.
//
//

#if defined(COMPILE_NATIVE)
#include "NativeWindow.h"

#include <array>

std::unordered_map<NativeWindowBasic*, NativeWindow*> NativeWindow::s_windowMap;

void nativeErrorCallback(int error, const char* description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void nativeWindowPosCallback(NativeWindowBasic* window, int xPos, int yPos) {
	NativeWindow::s_windowMap[window]->onMove(xPos, yPos);
}

void nativeFramebufferSizeCallback(NativeWindowBasic* window, int width, int height) {
	NativeWindow::s_windowMap[window]->onResize(width, height);
	// Verify framebuffer size
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	nativeGetFramebufferSize(NativeWindow::s_windowMap[window]->m_nativeWindow, &framebufferWidth, &framebufferHeight);
	printf("Framebuffer size: %d %d\n", framebufferWidth, framebufferHeight);
}

void nativeWindowFocusCallback(NativeWindowBasic* window, int focus) {
	NativeWindow::s_windowMap[window]->onFocus(focus);
}

void nativeWindowCloseCallback(NativeWindowBasic* window) {
	NativeWindow::s_windowMap[window]->onClose();
}

void nativeKeyCallback(NativeWindowBasic* window, int k, int s, int action, int mods) {
	if (action == NATIVE_RELEASE) {
		NativeWindow::s_windowMap[window]->onKeyUp(k);
	} else if (action == NATIVE_PRESS) {
		NativeWindow::s_windowMap[window]->onKeyDown(k);
	}
}

NativeWindow::NativeWindow(Parameters& parameters)
: AbstractWindow(parameters)
, m_nativeWindow(nullptr) {
	initializeNative();
	switch (m_type) {
		case DirectX: {
			initializeDirectX();
		} break;

		case Metal: {
			initializeMetal();
		} break;

		case OpenGL: {
			initializeOpenGL();
		} break;

		case Vulkan: {
			initializeVulkan();
		} break;

		default:
			break;
	}
}

NativeWindow::~NativeWindow() {
	switch (m_type) {
		case DirectX: {
			deinitializeDirectX();
		} break;

		case Metal: {
			deinitializeMetal();
		} break;

		case OpenGL: {
			deinitializeOpenGL();
		} break;

		case Vulkan: {
			deinitializeVulkan();
		} break;

		default:
			break;
	}
	deinitializeNative();
}

void NativeWindow::onResize(const int& xSize, const int& ySize) {
	AbstractWindow::onResize(xSize, ySize);
}

void NativeWindow::onFocus(const int& focus) {
	AbstractWindow::onFocus(focus);
}

void NativeWindow::update(float dt) {
	// do nothing
}

void NativeWindow::swapBuffers() {
	nativeSwapBuffers(m_nativeWindow);
}

void NativeWindow::pollEvents() {
	nativePollEvents();
}

void NativeWindow::getWindowPos(int& xPos, int& yPos) {
	nativeGetWindowPos(m_nativeWindow, &xPos, &yPos);
}

void NativeWindow::getWindowSize(int& width, int& height) {
	nativeGetWindowSize(m_nativeWindow, &width, &height);
}

void NativeWindow::getFramebufferSize(int& width, int& height) {
	nativeGetFramebufferSize(m_nativeWindow, &width, &height);
}

void NativeWindow::getMetalHackBoth(void** texture, void** drawable) {
	nativeGetMetalHackBoth(texture, drawable);
}

void NativeWindow::initializeNative() {
	nativeSetErrorCallback(nativeErrorCallback);
	nativeInit();
}

void NativeWindow::initializeDirectX() {
	nativeWindowHint(NATIVE_RESIZABLE, NATIVE_TRUE);

	// Create Context
	m_nativeWindow = nativeCreateWindowDirectX(m_xSize, m_ySize, m_title.c_str(), nullptr, nullptr);
	if (m_nativeWindow == nullptr) {
		printf("nativeCreateWindow Error: Unknown\n");
	}

	// Set callbacks
	nativeSetFramebufferSizeCallback(m_nativeWindow, &nativeFramebufferSizeCallback);
	nativeSetWindowPosCallback(m_nativeWindow, &nativeWindowPosCallback);
	nativeSetWindowFocusCallback(m_nativeWindow, &nativeWindowFocusCallback);
	nativeSetWindowCloseCallback(m_nativeWindow, &nativeWindowCloseCallback);
	nativeSetKeyCallback(m_nativeWindow, &nativeKeyCallback);

	nativeMakeContextCurrent(m_nativeWindow);

	// Insert entry into map
	s_windowMap[m_nativeWindow] = this;
}

void NativeWindow::initializeMetal() {
	nativeWindowHint(NATIVE_RESIZABLE, NATIVE_TRUE);

	// Create Window
	m_nativeWindow = nativeCreateWindowMetal(m_xSize, m_ySize, m_title.c_str(), nullptr, nullptr);
	if (m_nativeWindow == nullptr) {
		printf("nativeCreateWindow Error: Unknown\n");
	}

	// Set callbacks
	nativeSetFramebufferSizeCallback(m_nativeWindow, &nativeFramebufferSizeCallback);
	nativeSetWindowPosCallback(m_nativeWindow, &nativeWindowPosCallback);
	nativeSetWindowFocusCallback(m_nativeWindow, &nativeWindowFocusCallback);
	nativeSetWindowCloseCallback(m_nativeWindow, &nativeWindowCloseCallback);
	nativeSetKeyCallback(m_nativeWindow, &nativeKeyCallback);

	nativeMakeContextCurrent(m_nativeWindow);

	// Insert entry into map
	s_windowMap[m_nativeWindow] = this;
}

void NativeWindow::initializeOpenGL() {
	nativeWindowHint(NATIVE_RESIZABLE, NATIVE_TRUE);
	nativeWindowHint(NATIVE_OPENGL_PROFILE, NATIVE_OPENGL_CORE_PROFILE);
	nativeWindowHint(NATIVE_OPENGL_FORWARD_COMPAT, GL_TRUE);

#if defined(CONTEXT_MAJOR_VERSION) && defined(CONTEXT_MINOR_VERSION)
	// Create Context
	nativeWindowHint(NATIVE_CONTEXT_VERSION_MAJOR, CONTEXT_MAJOR_VERSION);
	nativeWindowHint(NATIVE_CONTEXT_VERSION_MINOR, CONTEXT_MINOR_VERSION);
	m_nativeWindow = nativeCreateWindow(m_xSize, m_ySize, m_title.c_str(), nullptr, nullptr);
	if (m_nativeWindow == nullptr) {
		printf("nativeCreateWindow Error: Unknown\n");
	} else {
		std::string version = std::to_string(CONTEXT_MAJOR_VERSION) + "." + std::to_string(CONTEXT_MINOR_VERSION);
		//..printf("OpenGL version: %s\n", version.c_str());
	}
#else
	// Create Context
	int major = 0;
	int minor = 0;
	int shader = 0;
	// The major, minor version of GL and Shader preprocessor
	const std::array<std::tuple<int, int, int>, 12> versions = {
		std::tuple<int, int, int>(4, 5, 450),
		std::tuple<int, int, int>(4, 4, 440),
		std::tuple<int, int, int>(4, 3, 430),
		std::tuple<int, int, int>(4, 2, 420),
		std::tuple<int, int, int>(4, 1, 410),
		std::tuple<int, int, int>(4, 0, 400),
		std::tuple<int, int, int>(3, 3, 330),
		std::tuple<int, int, int>(3, 2, 150),
		std::tuple<int, int, int>(3, 1, 140),
		std::tuple<int, int, int>(3, 0, 130),
		std::tuple<int, int, int>(2, 1, 120),
		std::tuple<int, int, int>(2, 0, 110)
	};
	for (auto version : versions) {
		std::tie(major, minor, std::ignore) = version;
		nativeWindowHint(NATIVE_CONTEXT_VERSION_MAJOR, major); // TODO
		nativeWindowHint(NATIVE_CONTEXT_VERSION_MINOR, minor); // TODO
		m_nativeWindow = nativeCreateWindowOpenGL(m_xSize, m_ySize, m_title.c_str(), nullptr, nullptr);
		if (m_nativeWindow != nullptr) {
			shader = std::get<2>(version);
			break;
		}
	}
	if (m_nativeWindow == nullptr) {
		printf("nativeCreateWindow Error: Unknown\n");
	} else {
		std::string version = std::to_string(major) + "." + std::to_string(minor) + ":" + std::to_string(shader);
		//..printf("OpenGL version: %s\n", version.c_str());
	}
#endif // defined(CONTEXT_MAJOR_VERSION) && defined(CONTEXT_MINOR_VERSION)

	// Set callbacks
	nativeSetFramebufferSizeCallback(m_nativeWindow, &nativeFramebufferSizeCallback);
	nativeSetWindowPosCallback(m_nativeWindow, &nativeWindowPosCallback);
	nativeSetWindowFocusCallback(m_nativeWindow, &nativeWindowFocusCallback);
	nativeSetWindowCloseCallback(m_nativeWindow, &nativeWindowCloseCallback);
	nativeSetKeyCallback(m_nativeWindow, &nativeKeyCallback);

	nativeMakeContextCurrent(m_nativeWindow);

	// Load symbols
	gladLoadGLLoader((GLADloadproc)nativeGetProcAddress);

	// Swap interval
	nativeSwapInterval(1);

	// Verify framebuffer size
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	nativeGetFramebufferSize(m_nativeWindow, &framebufferWidth, &framebufferHeight);
	printf("Framebuffer size: %d %d\n", framebufferWidth, framebufferHeight);

	// Insert entry into map
	s_windowMap[m_nativeWindow] = this;
}

void NativeWindow::initializeVulkan() {
	nativeWindowHint(NATIVE_RESIZABLE, NATIVE_TRUE);
	////nativeWindowHint(NATIVE_CLIENT_API, NATIVE_NO_API);

	// Create Context
	m_nativeWindow = nativeCreateWindowVulkan(m_xSize, m_ySize, m_title.c_str(), nullptr, nullptr);
	if (m_nativeWindow == nullptr) {
		printf("nativeCreateWindow Error: Unknown\n");
	}
#if defined(__linux__) || defined(_WIN32)
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = m_title.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t nativeExtensionCount = 0;
	const char** nativeExtensions;
	////nativeExtensions = nativeGetRequiredInstanceExtensions(&nativeExtensionCount);

	createInfo.enabledExtensionCount = nativeExtensionCount;
	createInfo.ppEnabledExtensionNames = nativeExtensions;

	createInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
#endif // defined(__linux__) || defined(_WIN32)
	// Set callbacks
	nativeSetFramebufferSizeCallback(m_nativeWindow, &nativeFramebufferSizeCallback);
	nativeSetWindowPosCallback(m_nativeWindow, &nativeWindowPosCallback);
	nativeSetWindowFocusCallback(m_nativeWindow, &nativeWindowFocusCallback);
	nativeSetWindowCloseCallback(m_nativeWindow, &nativeWindowCloseCallback);
	nativeSetKeyCallback(m_nativeWindow, &nativeKeyCallback);

	nativeMakeContextCurrent(m_nativeWindow);

	// Insert entry into map
	s_windowMap[m_nativeWindow] = this;
}

void NativeWindow::deinitializeNative() {
	nativeTerminate();
	nativeSetErrorCallback(nullptr);
}

void NativeWindow::deinitializeDirectX() {
	nativeDestroyWindow(m_nativeWindow);

	// Remove entry from map
	s_windowMap.erase(m_nativeWindow);
}

void NativeWindow::deinitializeMetal() {
	nativeDestroyWindow(m_nativeWindow);

	// Remove entry from map
	s_windowMap.erase(m_nativeWindow);
}

void NativeWindow::deinitializeOpenGL() {
	nativeDestroyWindow(m_nativeWindow);

	// Remove entry from map
	s_windowMap.erase(m_nativeWindow);
}

void NativeWindow::deinitializeVulkan() {
	nativeDestroyWindow(m_nativeWindow);

	// Remove entry from map
	s_windowMap.erase(m_nativeWindow);
}
#endif
