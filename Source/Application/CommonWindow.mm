//
//  CommonWindow.cpp
//
//
//  Created by Narendra Umate on 8/17/15.
//
//

#if defined(COMPILE_COMMON)
#include "CommonWindow.h"

#include <array>

#if defined(__linux__) || defined(_WIN32)
#include <vulkan/vulkan.h>
#else
#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>
#endif // defined(__linux__) || defined(_WIN32)

std::unordered_map<GLFWwindow*, CommonWindow*> CommonWindow::s_windowMap;

void glfwErrorCallback(int error, const char* description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void glfwWindowPosCallback(GLFWwindow* window, int xPos, int yPos) {
	CommonWindow::s_windowMap[window]->onMove(xPos, yPos);
}

void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	CommonWindow::s_windowMap[window]->onResize(width, height);
	// Verify framebuffer size
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	glfwGetFramebufferSize(CommonWindow::s_windowMap[window]->m_glfwWindow, &framebufferWidth, &framebufferHeight);
	printf("Framebuffer size: %d %d\n", framebufferWidth, framebufferHeight);
}

void glfwWindowFocusCallback(GLFWwindow* window, int focus) {
	CommonWindow::s_windowMap[window]->onFocus(focus);
}

void glfwWindowCloseCallback(GLFWwindow* window) {
	CommonWindow::s_windowMap[window]->onClose();
}

void glfwKeyCallback(GLFWwindow* window, int k, int s, int action, int mods) {
	if (action == GLFW_RELEASE) {
		CommonWindow::s_windowMap[window]->onKeyUp(k);
	} else if (action == GLFW_PRESS) {
		CommonWindow::s_windowMap[window]->onKeyDown(k);
	}
}

CommonWindow::CommonWindow(Parameters& parameters)
: AbstractWindow(parameters)
, m_glfwWindow(nullptr) {
	initializeCommon();
	switch (m_type) {
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

CommonWindow::~CommonWindow() {
	switch (m_type) {
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
	deinitializeCommon();
}

void CommonWindow::onResize(const int& xSize, const int& ySize) {
	AbstractWindow::onResize(xSize, ySize);
}

void CommonWindow::onFocus(const int& focus) {
	AbstractWindow::onFocus(focus);
}

void CommonWindow::update(float dt) {
	// do nothing
}

void CommonWindow::swapBuffers() {
	switch (m_type) {
        case Metal: {
            // do nothing
        } break;

        case OpenGL: {
			glfwSwapBuffers(m_glfwWindow);
		} break;

		case Vulkan: {
            // do nothing
        } break;

		default:
			break;
	}
}

void CommonWindow::pollEvents() {
	glfwPollEvents();
}

void CommonWindow::getWindowPos(int& xPos, int& yPos) {
	glfwGetWindowPos(m_glfwWindow, &xPos, &yPos);
}

void CommonWindow::getWindowSize(int& width, int& height) {
	glfwGetWindowSize(m_glfwWindow, &width, &height);
}

void CommonWindow::getFramebufferSize(int& width, int& height) {
	glfwGetFramebufferSize(m_glfwWindow, &width, &height);
}

void CommonWindow::getMetalHackBoth(void** texture, void** drawable)
{
    if (m_type == Metal) {
#if defined(__APPLE__)
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(m_glfwWindow, &width, &height);
        CAMetalLayer* currentLayer = (__bridge CAMetalLayer*)(m_layer);
        currentLayer.drawableSize = CGSizeMake(width, height);
        id<CAMetalDrawable> nextDrawable = [currentLayer nextDrawable];
        *drawable = (void*)CFBridgingRetain(nextDrawable);
        id<MTLTexture> nextTexture = [nextDrawable texture];
        *texture = (void*)CFBridgingRetain(nextTexture);
#else
	    *texture = 0;
    	*drawable = 0;
#endif
    }
}

void CommonWindow::initializeCommon() {
	glfwSetErrorCallback(glfwErrorCallback);
	glfwInit();
}

void CommonWindow::initializeMetal() {
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Create Window
    m_glfwWindow = glfwCreateWindow(m_xSize, m_ySize, m_title.c_str(), nullptr, nullptr);
    if (m_glfwWindow == nullptr) {
        printf("glfwCreateWindow Error: Unknown\n");
    }
#if defined(__APPLE__)
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    NSWindow* nswin = glfwGetCocoaWindow(m_glfwWindow);
    CAMetalLayer* currentLayer = [CAMetalLayer layer];
    currentLayer.device = device;
    currentLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    currentLayer.framebufferOnly = NO;
    nswin.contentView.layer = currentLayer;
    nswin.contentView.wantsLayer = YES;
    m_layer = (void*)CFBridgingRetain(currentLayer);
#endif
	// Set callbacks
	glfwSetFramebufferSizeCallback(m_glfwWindow, &glfwFramebufferSizeCallback);
	glfwSetWindowPosCallback(m_glfwWindow, &glfwWindowPosCallback);
	glfwSetWindowFocusCallback(m_glfwWindow, &glfwWindowFocusCallback);
	glfwSetWindowCloseCallback(m_glfwWindow, &glfwWindowCloseCallback);
	glfwSetKeyCallback(m_glfwWindow, &glfwKeyCallback);

	glfwMakeContextCurrent(m_glfwWindow);

	// Insert entry into map
	s_windowMap[m_glfwWindow] = this;
}

void CommonWindow::initializeOpenGL() {
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#if defined(CONTEXT_MAJOR_VERSION) && defined(CONTEXT_MINOR_VERSION)
	// Create Context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_MAJOR_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_MINOR_VERSION);
	m_glfwWindow = glfwCreateWindow(m_xSize, m_ySize, m_title.c_str(), nullptr, nullptr);
	if (m_glfwWindow == nullptr) {
		printf("glfwCreateWindow Error: Unknown\n");
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
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
		m_glfwWindow = glfwCreateWindow(m_xSize, m_ySize, m_title.c_str(), nullptr, nullptr);
		if (m_glfwWindow != nullptr) {
			shader = std::get<2>(version);
			break;
		}
	}
	if (m_glfwWindow == nullptr) {
		printf("glfwCreateWindow Error: Unknown\n");
	} else {
		std::string version = std::to_string(major) + "." + std::to_string(minor) + ":" + std::to_string(shader);
		//..printf("OpenGL version: %s\n", version.c_str());
	}
#endif // defined(CONTEXT_MAJOR_VERSION) && defined(CONTEXT_MINOR_VERSION)

	// Set callbacks
	glfwSetFramebufferSizeCallback(m_glfwWindow, &glfwFramebufferSizeCallback);
	glfwSetWindowPosCallback(m_glfwWindow, &glfwWindowPosCallback);
	glfwSetWindowFocusCallback(m_glfwWindow, &glfwWindowFocusCallback);
	glfwSetWindowCloseCallback(m_glfwWindow, &glfwWindowCloseCallback);
	glfwSetKeyCallback(m_glfwWindow, &glfwKeyCallback);

	glfwMakeContextCurrent(m_glfwWindow);

	// Load symbols
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Swap interval
	glfwSwapInterval(1);

	// Verify framebuffer size
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	glfwGetFramebufferSize(m_glfwWindow, &framebufferWidth, &framebufferHeight);
	printf("Framebuffer size: %d %d\n", framebufferWidth, framebufferHeight);

	// Insert entry into map
	s_windowMap[m_glfwWindow] = this;
}

void CommonWindow::initializeVulkan() {
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Create Context
	m_glfwWindow = glfwCreateWindow(m_xSize, m_ySize, m_title.c_str(), nullptr, nullptr);
	if (m_glfwWindow == nullptr) {
		printf("glfwCreateWindow Error: Unknown\n");
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

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

	createInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
#endif // defined(__linux__) || defined(_WIN32)
	// Set callbacks
	glfwSetFramebufferSizeCallback(m_glfwWindow, &glfwFramebufferSizeCallback);
	glfwSetWindowPosCallback(m_glfwWindow, &glfwWindowPosCallback);
	glfwSetWindowFocusCallback(m_glfwWindow, &glfwWindowFocusCallback);
	glfwSetWindowCloseCallback(m_glfwWindow, &glfwWindowCloseCallback);
	glfwSetKeyCallback(m_glfwWindow, &glfwKeyCallback);
#if 0
	glfwMakeContextCurrent(m_glfwWindow); // This is an Open GL only thing.
#else
#if defined(__linux__) || defined(_WIN32)
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::cout << extensionCount << " extensions supported\n";
#endif // defined(__linux__) || defined(_WIN32)
#endif
	// Insert entry into map
	s_windowMap[m_glfwWindow] = this;
}

void CommonWindow::deinitializeCommon() {
	glfwTerminate();
	glfwSetErrorCallback(nullptr);
}

void CommonWindow::deinitializeMetal() {
    glfwDestroyWindow(m_glfwWindow);

    // Remove entry from map
    s_windowMap.erase(m_glfwWindow);
}

void CommonWindow::deinitializeOpenGL() {
	glfwDestroyWindow(m_glfwWindow);

	// Remove entry from map
	s_windowMap.erase(m_glfwWindow);
}

void CommonWindow::deinitializeVulkan() {
#if defined(__linux__) || defined(_WIN32)
	vkDestroyInstance(instance, nullptr);
#endif // defined(__linux__) || defined(_WIN32)

    glfwDestroyWindow(m_glfwWindow);

	// Remove entry from map
	s_windowMap.erase(m_glfwWindow);
}

#endif
