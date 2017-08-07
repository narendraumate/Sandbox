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

#include "VulkanSample.h"

#include "Shaders.h"
#include "Window.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#pragma warning(disable : 4100) // disable unreferenced formal parameter warnings

namespace {
///////////////////////////////////////////////////////////////////////////////
VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(
	VkDebugReportFlagsEXT /*flags*/,
	VkDebugReportObjectTypeEXT /*objectType*/,
	uint64_t /*object*/,
	size_t /*location*/,
	int32_t /*messageCode*/,
	const char* /*pLayerPrefix*/,
	const char* pMessage,
	void* /*pUserData*/) {
#if defined(_WIN32)
	OutputDebugStringA(pMessage);
	OutputDebugStringA("\n");
#endif
	return VK_FALSE;
}

///////////////////////////////////////////////////////////////////////////////
std::vector<const char*> GetDebugInstanceLayerNames() {
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> instanceLayers{ layerCount };
	vkEnumerateInstanceLayerProperties(&layerCount, instanceLayers.data());

	std::vector<const char*> result;
	for (const auto& p : instanceLayers) {
		if (strcmp(p.layerName, "VK_LAYER_LUNARG_standard_validation") == 0) {
			result.push_back("VK_LAYER_LUNARG_standard_validation");
		}
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////
std::vector<const char*> GetDebugInstanceExtensionNames() {
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> instanceExtensions{ extensionCount };
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, instanceExtensions.data());

	std::vector<const char*> result;
	for (const auto& e : instanceExtensions) {
		if (strcmp(e.extensionName, "VK_EXT_debug_report") == 0) {
			result.push_back("VK_EXT_debug_report");
		}
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////
std::vector<const char*> GetDebugDeviceLayerNames(VkPhysicalDevice device) {
	uint32_t layerCount = 0;
	vkEnumerateDeviceLayerProperties(device, &layerCount, nullptr);

	std::vector<VkLayerProperties> deviceLayers{ layerCount };
	vkEnumerateDeviceLayerProperties(device, &layerCount, deviceLayers.data());

	std::vector<const char*> result;
	for (const auto& p : deviceLayers) {
		if (strcmp(p.layerName, "VK_LAYER_LUNARG_standard_validation") == 0) {
			result.push_back("VK_LAYER_LUNARG_standard_validation");
		}
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////
void FindPhysicalDeviceWithGraphicsQueue(const std::vector<VkPhysicalDevice>& physicalDevices,
										 VkPhysicalDevice* outputDevice, int* outputGraphicsQueueIndex) {
	for (auto physicalDevice : physicalDevices) {
		uint32_t queueFamilyPropertyCount = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilyProperties{ queueFamilyPropertyCount };
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

		int i = 0;
		for (const auto& queueFamilyProperty : queueFamilyProperties) {
			if (queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				if (outputDevice) {
					*outputDevice = physicalDevice;
				}

				if (outputGraphicsQueueIndex) {
					*outputGraphicsQueueIndex = i;
				}

				return;
			}

			++i;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
VkInstance CreateInstance() {
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#if defined(_WIN32)
	std::vector<const char*> instanceExtensions = {
		"VK_KHR_surface", "VK_KHR_win32_surface"
	};
#else
	std::vector<const char*> instanceExtensions = {
		"VK_KHR_surface", "VK_KHR_xcb_surface"
	};
#endif

#ifdef _DEBUG
	auto debugInstanceExtensionNames = GetDebugInstanceExtensionNames();
	instanceExtensions.insert(instanceExtensions.end(), debugInstanceExtensionNames.begin(), debugInstanceExtensionNames.end());
#endif

	instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());

	std::vector<const char*> instanceLayers;

#ifdef _DEBUG
	auto debugInstanceLayerNames = GetDebugInstanceLayerNames();
	instanceLayers.insert(instanceLayers.end(), debugInstanceLayerNames.begin(), debugInstanceLayerNames.end());
#endif

	instanceCreateInfo.ppEnabledLayerNames = instanceLayers.data();
	instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(instanceLayers.size());

	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.apiVersion = VK_API_VERSION_1_0;
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pApplicationName = "Vulkan Sample application";
	applicationInfo.pEngineName = "Vulkan Sample Engine";

	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	VkInstance instance = VK_NULL_HANDLE;
	vkCreateInstance(&instanceCreateInfo, nullptr, &instance);

	return instance;
}

///////////////////////////////////////////////////////////////////////////////
void CreateDeviceAndQueue(VkInstance instance, VkDevice* outputDevice,
						  VkQueue* outputQueue, int* outputQueueIndex,
						  VkPhysicalDevice* outputPhysicalDevice) {
	uint32_t physicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> devices{ physicalDeviceCount };
	vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices.data());

	VkPhysicalDevice physicalDevice = nullptr;
	int graphicsQueueIndex = -1;

	FindPhysicalDeviceWithGraphicsQueue(devices, &physicalDevice, &graphicsQueueIndex);

	assert(physicalDevice);

	VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.queueCount = 1;
	deviceQueueCreateInfo.queueFamilyIndex = graphicsQueueIndex;

	static const float queuePriorities[] = { 1.0f };
	deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;

	std::vector<const char*> deviceLayers;

#ifdef _DEBUG
	auto debugDeviceLayerNames = GetDebugDeviceLayerNames(physicalDevice);
	deviceLayers.insert(deviceLayers.end(), debugDeviceLayerNames.begin(), debugDeviceLayerNames.end());
#endif

	deviceCreateInfo.ppEnabledLayerNames = deviceLayers.data();
	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(deviceLayers.size());

	// Our SPIR-V requests this, so we need to enable it here
	VkPhysicalDeviceFeatures enabledFeatures = {};
	enabledFeatures.shaderClipDistance = true;

	deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

	std::vector<const char*> deviceExtensions = {
		"VK_KHR_swapchain"
	};

	deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());

	VkDevice device = nullptr;
	vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);
	assert(device);

	VkQueue queue = nullptr;
	vkGetDeviceQueue(device, graphicsQueueIndex, 0, &queue);
	assert(queue);

	if (outputQueue) {
		*outputQueue = queue;
	}

	if (outputDevice) {
		*outputDevice = device;
	}

	if (outputQueueIndex) {
		*outputQueueIndex = graphicsQueueIndex;
	}

	if (outputPhysicalDevice) {
		*outputPhysicalDevice = physicalDevice;
	}
}

struct SwapchainFormatColorSpace {
	VkFormat format;
	VkColorSpaceKHR colorSpace;
};

///////////////////////////////////////////////////////////////////////////////
SwapchainFormatColorSpace GetSwapchainFormatAndColorspace(VkPhysicalDevice physicalDevice,
														  VkSurfaceKHR surface, VulkanSample::ImportTable* importTable) {
	uint32_t surfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr);

	std::vector<VkSurfaceFormatKHR> surfaceFormats{ surfaceFormatCount };
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data());

	SwapchainFormatColorSpace result;

	if (surfaceFormatCount == 1 && surfaceFormats.front().format == VK_FORMAT_UNDEFINED) {
		result.format = VK_FORMAT_R8G8B8A8_UNORM;
	} else {
		result.format = surfaceFormats.front().format;
	}

	result.colorSpace = surfaceFormats.front().colorSpace;

	return result;
}

///////////////////////////////////////////////////////////////////////////////
VkRenderPass CreateRenderPass(VkDevice device, VkFormat swapchainFormat) {
	VkAttachmentDescription attachmentDescription = {};
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.format = swapchainFormat;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	VkAttachmentReference attachmentReference = {};
	attachmentReference.attachment = 0;
	attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpassDescription = {};
	subpassDescription.inputAttachmentCount = 0;
	subpassDescription.pColorAttachments = &attachmentReference;
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpassDescription;
	renderPassCreateInfo.pAttachments = &attachmentDescription;

	VkRenderPass result = 0;
	vkCreateRenderPass(device, &renderPassCreateInfo, 0, &result);

	return result;
}

///////////////////////////////////////////////////////////////////////////////
void CreateFramebuffers(VkDevice device, VkRenderPass renderPass,
						const int width, const int height,
						const int count, const VkImageView* imageViews, VkFramebuffer* framebuffers) {
	for (int i = 0; i < count; ++i) {
		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = &imageViews[i];
		framebufferCreateInfo.height = height;
		framebufferCreateInfo.width = width;
		framebufferCreateInfo.layers = 1;
		framebufferCreateInfo.renderPass = renderPass;
		vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &framebuffers[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
void CreateSwapchainImageViews(VkDevice device, VkFormat format,
							   const int count, const VkImage* images, VkImageView* imageViews) {
	for (int i = 0; i < count; ++i) {
		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = images[i];
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = format;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.layerCount = 1;
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		vkCreateImageView(device, &imageViewCreateInfo, nullptr, &imageViews[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
VkSwapchainKHR CreateSwapchain(VkPhysicalDevice physicalDevice, VkDevice device,
							   VkSurfaceKHR surface, const int surfaceWidth, const int surfaceHeight,
							   const int backbufferCount, VulkanSample::ImportTable* importTable,
							   VkFormat* swapchainFormat) {
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

	std::vector<VkPresentModeKHR> presentModes{ presentModeCount };

	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

	VkExtent2D swapChainSize = {};
	swapChainSize = surfaceCapabilities.currentExtent;
	assert(static_cast<int>(swapChainSize.width) == surfaceWidth);
	assert(static_cast<int>(swapChainSize.height) == surfaceHeight);

	uint32_t swapChainImageCount = backbufferCount;
	assert(swapChainImageCount >= surfaceCapabilities.minImageCount);

	// 0 indicates unlimited number of images
	if (surfaceCapabilities.maxImageCount != 0) {
		assert(swapChainImageCount < surfaceCapabilities.maxImageCount);
	}

	VkSurfaceTransformFlagBitsKHR surfaceTransformFlags;

	if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
		surfaceTransformFlags = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	} else {
		surfaceTransformFlags = surfaceCapabilities.currentTransform;
	}

	auto swapchainFormatColorSpace = GetSwapchainFormatAndColorspace(physicalDevice, surface, importTable);

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = swapChainImageCount;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.preTransform = surfaceTransformFlags;
	swapchainCreateInfo.imageColorSpace = swapchainFormatColorSpace.colorSpace;
	swapchainCreateInfo.imageFormat = swapchainFormatColorSpace.format;
	swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.imageExtent = swapChainSize;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

	VkSwapchainKHR swapchain;
	vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain);

	if (swapchainFormat) {
		*swapchainFormat = swapchainFormatColorSpace.format;
	}

	return swapchain;
}

///////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
VkSurfaceKHR CreateSurface(VkInstance instance, HWND hwnd, HINSTANCE hinstance) {
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hwnd = hwnd;
	surfaceCreateInfo.hinstance = hinstance;

	VkSurfaceKHR surface = 0;
	vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);

	return surface;
}
#else
VkSurfaceKHR CreateSurface(VkInstance instance, xcb_connection_t* connection, xcb_window_t window) {
	VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.connection = connection;
	surfaceCreateInfo.window = window;

	VkSurfaceKHR surface = 0;
	vkCreateXcbSurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface);

	return surface;
}
#endif

#ifdef _DEBUG
///////////////////////////////////////////////////////////////////////////////
VkDebugReportCallbackEXT SetupDebugCallback(VkInstance instance, VulkanSample::ImportTable* importTable) {
	if (importTable->vkCreateDebugReportCallbackEXT) {
		VkDebugReportCallbackCreateInfoEXT callbackCreateInfo = {};
		callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		callbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		callbackCreateInfo.pfnCallback = &DebugReportCallback;

		VkDebugReportCallbackEXT callback;
		importTable->vkCreateDebugReportCallbackEXT(instance, &callbackCreateInfo, nullptr, &callback);
		return callback;
	} else {
		return VK_NULL_HANDLE;
	}
}

///////////////////////////////////////////////////////////////////////////////
void CleanupDebugCallback(VkInstance instance, VkDebugReportCallbackEXT callback,
						  VulkanSample::ImportTable* importTable) {
	if (importTable->vkDestroyDebugReportCallbackEXT) {
		importTable->vkDestroyDebugReportCallbackEXT(instance, callback, nullptr);
	}
}
#endif

struct MemoryTypeInfo {
	bool deviceLocal = false;
	bool hostVisible = false;
	bool hostCoherent = false;
	bool hostCached = false;
	bool lazilyAllocated = false;

	struct Heap {
		uint64_t size = 0;
		bool deviceLocal = false;
	};

	Heap heap;
	int index;
};

///////////////////////////////////////////////////////////////////////////////
std::vector<MemoryTypeInfo> EnumerateHeaps(VkPhysicalDevice device) {
	VkPhysicalDeviceMemoryProperties memoryProperties = {};
	vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

	std::vector<MemoryTypeInfo::Heap> heaps;

	for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i) {
		MemoryTypeInfo::Heap info;
		info.size = memoryProperties.memoryHeaps[i].size;
		info.deviceLocal = (memoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0;

		heaps.push_back(info);
	}

	std::vector<MemoryTypeInfo> result;

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
		MemoryTypeInfo typeInfo;

		typeInfo.deviceLocal = (memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0;
		typeInfo.hostVisible = (memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;
		typeInfo.hostCoherent = (memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0;
		typeInfo.hostCached = (memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0;
		typeInfo.lazilyAllocated = (memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) != 0;

		typeInfo.heap = heaps[memoryProperties.memoryTypes[i].heapIndex];

		typeInfo.index = static_cast<int>(i);

		result.push_back(typeInfo);
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////
VkDeviceMemory AllocateMemory(const std::vector<MemoryTypeInfo>& memoryInfos,
							  VkDevice device, const int size, bool* isHostCoherent = nullptr) {
	// We take the first HOST_VISIBLE memory
	for (auto& memoryInfo : memoryInfos) {
		if (memoryInfo.hostVisible) {
			VkMemoryAllocateInfo memoryAllocateInfo = {};
			memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			memoryAllocateInfo.memoryTypeIndex = memoryInfo.index;
			memoryAllocateInfo.allocationSize = size;

			VkDeviceMemory deviceMemory;
			vkAllocateMemory(device, &memoryAllocateInfo, nullptr,
							 &deviceMemory);

			if (isHostCoherent) {
				*isHostCoherent = memoryInfo.hostCoherent;
			}

			return deviceMemory;
		}
	}

	return VK_NULL_HANDLE;
}

///////////////////////////////////////////////////////////////////////////////
VkBuffer AllocateBuffer(VkDevice device, const int size,
						const VkBufferUsageFlagBits bits) {
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = static_cast<uint32_t>(size);
	bufferCreateInfo.usage = bits;

	VkBuffer result;
	vkCreateBuffer(device, &bufferCreateInfo, nullptr, &result);
	return result;
}

///////////////////////////////////////////////////////////////////////////////
VkPipelineLayout CreatePipelineLayout(VkDevice device) {
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	VkPipelineLayout result;
	vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr,
						   &result);

	return result;
}

///////////////////////////////////////////////////////////////////////////////
VkShaderModule LoadShader(VkDevice device, const void* shaderContents,
						  const size_t size) {
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

	shaderModuleCreateInfo.pCode = static_cast<const uint32_t*>(shaderContents);
	shaderModuleCreateInfo.codeSize = size;

	VkShaderModule result;
	vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &result);

	return result;
}

///////////////////////////////////////////////////////////////////////////////
VkDeviceSize RoundToNextMultiple(const VkDeviceSize& a, const VkDeviceSize& multiple) {
	return ((a + multiple - 1) / multiple) * multiple;
}

///////////////////////////////////////////////////////////////////////////////
VkPipeline CreatePipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout layout,
						  VkShaderModule vertexShader, VkShaderModule fragmentShader,
						  VkExtent2D viewportSize) {
	VkVertexInputBindingDescription vertexInputBindingDescription;
	vertexInputBindingDescription.binding = 0;
	vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	vertexInputBindingDescription.stride = sizeof(float) * 5;

	VkVertexInputAttributeDescription vertexInputAttributeDescription[2] = {};
	vertexInputAttributeDescription[0].binding = vertexInputBindingDescription.binding;
	vertexInputAttributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	vertexInputAttributeDescription[0].location = 0;
	vertexInputAttributeDescription[0].offset = 0;

	vertexInputAttributeDescription[1].binding = vertexInputBindingDescription.binding;
	vertexInputAttributeDescription[1].format = VK_FORMAT_R32G32_SFLOAT;
	vertexInputAttributeDescription[1].location = 1;
	vertexInputAttributeDescription[1].offset = sizeof(float) * 3;

	VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {};
	pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = std::extent<decltype(vertexInputAttributeDescription)>::value;
	pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescription;
	pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
	pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = &vertexInputBindingDescription;

	VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {};
	pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {};
	pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

	VkViewport viewport;
	viewport.height = static_cast<float>(viewportSize.height);
	viewport.width = static_cast<float>(viewportSize.width);
	viewport.x = 0;
	viewport.y = 0;
	viewport.minDepth = 0;
	viewport.maxDepth = 1;

	pipelineViewportStateCreateInfo.viewportCount = 1;
	pipelineViewportStateCreateInfo.pViewports = &viewport;

	VkRect2D rect;
	rect.extent = viewportSize;
	rect.offset.x = 0;
	rect.offset.y = 0;

	pipelineViewportStateCreateInfo.scissorCount = 1;
	pipelineViewportStateCreateInfo.pScissors = &rect;

	VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = {};
	pipelineColorBlendAttachmentState.colorWriteMask = 0xF;
	pipelineColorBlendAttachmentState.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {};
	pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

	pipelineColorBlendStateCreateInfo.attachmentCount = 1;
	pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;

	VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = {};
	pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	pipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_NONE;
	pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;

	VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo = {};
	pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_FALSE;
	pipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_FALSE;
	pipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_ALWAYS;
	pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
	pipelineDepthStencilStateCreateInfo.back.failOp = VK_STENCIL_OP_KEEP;
	pipelineDepthStencilStateCreateInfo.back.passOp = VK_STENCIL_OP_KEEP;
	pipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
	pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
	pipelineDepthStencilStateCreateInfo.front = pipelineDepthStencilStateCreateInfo.back;

	VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo = {};
	pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2] = {};
	pipelineShaderStageCreateInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineShaderStageCreateInfos[0].module = vertexShader;
	pipelineShaderStageCreateInfos[0].pName = "main";
	pipelineShaderStageCreateInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;

	pipelineShaderStageCreateInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineShaderStageCreateInfos[1].module = fragmentShader;
	pipelineShaderStageCreateInfos[1].pName = "main";
	pipelineShaderStageCreateInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	graphicsPipelineCreateInfo.layout = layout;
	graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.renderPass = renderPass;
	graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
	graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = &pipelineDepthStencilStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
	graphicsPipelineCreateInfo.pStages = pipelineShaderStageCreateInfos;
	graphicsPipelineCreateInfo.stageCount = 2;

	VkPipeline pipeline;
	vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo,
							  nullptr, &pipeline);

	return pipeline;
}

} // namespace

///////////////////////////////////////////////////////////////////////////////
VulkanSample::VulkanSample() {
	instance_ = CreateInstance();
	if (instance_ == VK_NULL_HANDLE) {
		// just bail out if the user does not have a compatible Vulkan driver
		return;
	}

	VkPhysicalDevice physicalDevice;
	CreateDeviceAndQueue(instance_, &device_, &queue_, &queueFamilyIndex_, &physicalDevice);
	physicalDevice_ = physicalDevice;

	importTable_.reset(new ImportTable{ instance_, device_ });

#ifdef _DEBUG
	debugCallback_ = SetupDebugCallback(instance_, importTable_.get());
#endif

	window_.reset(new Window{ "Puzzle Vulkan", 800, 600 });
#if defined(_WIN32)
	surface_ = CreateSurface(instance_, window_->GetHWND(), window_->GetHINSTANCE());
#else
	surface_ = CreateSurface(instance_, window_->GetConnection(), window_->GetWindow());
#endif
	VkBool32 presentSupported;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 0, surface_, &presentSupported);
	assert(presentSupported);

	VkFormat swapchainFormat = VK_FORMAT_UNDEFINED;
	swapchain_ = CreateSwapchain(physicalDevice, device_, surface_, window_->GetWidth(), window_->GetHeight(), QUEUE_SLOT_COUNT, importTable_.get(), &swapchainFormat);

	assert(swapchain_);

	uint32_t swapchainImageCount = 0;
	vkGetSwapchainImagesKHR(device_, swapchain_, &swapchainImageCount, nullptr);
	assert(static_cast<int>(swapchainImageCount) == QUEUE_SLOT_COUNT);

	vkGetSwapchainImagesKHR(device_, swapchain_, &swapchainImageCount, swapchainImages_);

	renderPass_ = CreateRenderPass(device_, swapchainFormat);

	CreateSwapchainImageViews(device_, swapchainFormat, QUEUE_SLOT_COUNT, swapchainImages_, swapChainImageViews_);
	CreateFramebuffers(device_, renderPass_, window_->GetWidth(), window_->GetHeight(), QUEUE_SLOT_COUNT, swapChainImageViews_, framebuffer_);

	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex_;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	vkCreateCommandPool(device_, &commandPoolCreateInfo, nullptr, &commandPool_);

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandBufferCount = QUEUE_SLOT_COUNT + 1;
	commandBufferAllocateInfo.commandPool = commandPool_;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	VkCommandBuffer commandBuffers[QUEUE_SLOT_COUNT + 1];

	vkAllocateCommandBuffers(device_, &commandBufferAllocateInfo, commandBuffers);

	for (int i = 0; i < QUEUE_SLOT_COUNT; ++i) {
		commandBuffers_[i] = commandBuffers[i];
	}

	setupCommandBuffer_ = commandBuffers[QUEUE_SLOT_COUNT];

	for (int i = 0; i < QUEUE_SLOT_COUNT; ++i) {
		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		// We need this so we can wait for them on the first try
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		vkCreateFence(device_, &fenceCreateInfo, nullptr, &frameFences_[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
VulkanSample::~VulkanSample() {
	for (int i = 0; i < QUEUE_SLOT_COUNT; ++i) {
		vkDestroyFence(device_, frameFences_[i], nullptr);
	}

	vkDestroyRenderPass(device_, renderPass_, nullptr);

	for (int i = 0; i < QUEUE_SLOT_COUNT; ++i) {
		vkDestroyFramebuffer(device_, framebuffer_[i], nullptr);
		vkDestroyImageView(device_, swapChainImageViews_[i], nullptr);
	}

	vkDestroyCommandPool(device_, commandPool_, nullptr);

	vkDestroySwapchainKHR(device_, swapchain_, nullptr);
	vkDestroySurfaceKHR(instance_, surface_, nullptr);

#ifdef _DEBUG
	CleanupDebugCallback(instance_, debugCallback_, importTable_.get());
#endif

	vkDestroyDevice(device_, nullptr);
	vkDestroyInstance(instance_, nullptr);
}

///////////////////////////////////////////////////////////////////////////////
void VulkanSample::Before() {
	vkResetFences(device_, 1, &frameFences_[0]);
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		vkBeginCommandBuffer(setupCommandBuffer_, &beginInfo);

		InitializeImpl(setupCommandBuffer_);

		vkEndCommandBuffer(setupCommandBuffer_);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &setupCommandBuffer_;
		vkQueueSubmit(queue_, 1, &submitInfo, frameFences_[0]);
	}

	vkWaitForFences(device_, 1, &frameFences_[0], VK_TRUE, UINT64_MAX);

	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	vkCreateSemaphore(device_, &semaphoreCreateInfo, nullptr, &imageAcquiredSemaphore);

	vkCreateSemaphore(device_, &semaphoreCreateInfo, nullptr, &renderingCompleteSemaphore);
}

///////////////////////////////////////////////////////////////////////////////
void VulkanSample::After() {
	// Wait for all rendering to finish
	vkWaitForFences(device_, 3, frameFences_, VK_TRUE, UINT64_MAX);

	vkDestroySemaphore(device_, imageAcquiredSemaphore, nullptr);
	vkDestroySemaphore(device_, renderingCompleteSemaphore, nullptr);

	ShutdownImpl();
}

///////////////////////////////////////////////////////////////////////////////
void VulkanSample::Run() {
	vkAcquireNextImageKHR(device_, swapchain_, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &currentBackBuffer_);

	vkWaitForFences(device_, 1, &frameFences_[currentBackBuffer_], VK_TRUE, UINT64_MAX);
	vkResetFences(device_, 1, &frameFences_[currentBackBuffer_]);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	vkBeginCommandBuffer(commandBuffers_[currentBackBuffer_], &beginInfo);

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.framebuffer = framebuffer_[currentBackBuffer_];
	renderPassBeginInfo.renderArea.extent.width = window_->GetWidth();
	renderPassBeginInfo.renderArea.extent.height = window_->GetHeight();
	renderPassBeginInfo.renderPass = renderPass_;

	VkClearValue clearValue = {};

	clearValue.color.float32[0] = 0.042f;
	clearValue.color.float32[1] = 0.042f;
	clearValue.color.float32[2] = 0.042f;
	clearValue.color.float32[3] = 1.0f;

	renderPassBeginInfo.pClearValues = &clearValue;
	renderPassBeginInfo.clearValueCount = 1;

	vkCmdBeginRenderPass(commandBuffers_[currentBackBuffer_], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	RenderImpl(commandBuffers_[currentBackBuffer_]);

	vkCmdEndRenderPass(commandBuffers_[currentBackBuffer_]);
	vkEndCommandBuffer(commandBuffers_[currentBackBuffer_]);

	// Submit rendering work to the graphics queue
	const VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &imageAcquiredSemaphore;
	submitInfo.pWaitDstStageMask = &waitDstStageMask;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers_[currentBackBuffer_];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderingCompleteSemaphore;
	vkQueueSubmit(queue_, 1, &submitInfo, VK_NULL_HANDLE);

	// Submit present operation to present queue
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderingCompleteSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapchain_;
	presentInfo.pImageIndices = &currentBackBuffer_;
	vkQueuePresentKHR(queue_, &presentInfo);

	vkQueueSubmit(queue_, 0, nullptr, frameFences_[currentBackBuffer_]);
}

///////////////////////////////////////////////////////////////////////////////
void VulkanSample::InitializeImpl(VkCommandBuffer uploadCommandBuffer) {
	////VulkanSample::InitializeImpl (uploadCommandBuffer);

	CreatePipelineStateObject();
	CreateMeshBuffers(uploadCommandBuffer);
}

///////////////////////////////////////////////////////////////////////////////
void VulkanSample::RenderImpl(VkCommandBuffer commandBuffer) {
	////VulkanSample::RenderImpl (commandBuffer);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
					  pipeline_);
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindIndexBuffer(commandBuffer, indexBuffer_, 0, VK_INDEX_TYPE_UINT32);
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer_, offsets);
	vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////
void VulkanSample::ShutdownImpl() {
	vkDestroyPipeline(device_, pipeline_, nullptr);
	vkDestroyPipelineLayout(device_, pipelineLayout_, nullptr);

	vkDestroyBuffer(device_, vertexBuffer_, nullptr);
	vkDestroyBuffer(device_, indexBuffer_, nullptr);
	vkFreeMemory(device_, deviceMemory_, nullptr);

	vkDestroyShaderModule(device_, vertexShader_, nullptr);
	vkDestroyShaderModule(device_, fragmentShader_, nullptr);

	////VulkanSample::ShutdownImpl ();
}

///////////////////////////////////////////////////////////////////////////////
void /*VulkanQuad*/ VulkanSample::CreateMeshBuffers(VkCommandBuffer uploadCommandBuffer) {
	struct Vertex {
		float position[3];
		float uv[2];
	};

	static const Vertex vertices[4] = {
		// Upper Left
		{ { -1.0f, 1.0f, 0 }, { 0, 0 } },
		// Upper Right
		{ { 1.0f, 1.0f, 0 }, { 1, 0 } },
		// Bottom right
		{ { 1.0f, -1.0f, 0 }, { 1, 1 } },
		// Bottom left
		{ { -1.0f, -1.0f, 0 }, { 0, 1 } }
	};

	static const int indices[6] = {
		0, 1, 2, 2, 3, 0
	};

	auto memoryHeaps = EnumerateHeaps(physicalDevice_);
	indexBuffer_ = AllocateBuffer(device_, sizeof(indices),
								  VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
	vertexBuffer_ = AllocateBuffer(device_, sizeof(vertices),
								   VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
	VkMemoryRequirements vertexBufferMemoryRequirements = {};
	vkGetBufferMemoryRequirements(device_, vertexBuffer_,
								  &vertexBufferMemoryRequirements);
	VkMemoryRequirements indexBufferMemoryRequirements = {};
	vkGetBufferMemoryRequirements(device_, indexBuffer_,
								  &indexBufferMemoryRequirements);

	VkDeviceSize bufferSize = vertexBufferMemoryRequirements.size;
	// We want to place the index buffer behind the vertex buffer. Need to take
	// the alignment into account to find the next suitable location
	VkDeviceSize indexBufferOffset = RoundToNextMultiple(bufferSize,
														 indexBufferMemoryRequirements.alignment);

	bufferSize = indexBufferOffset + indexBufferMemoryRequirements.size;
	bool memoryIsHostCoherent = false;
	deviceMemory_ = AllocateMemory(memoryHeaps, device_,
								   static_cast<int>(bufferSize), &memoryIsHostCoherent);

	vkBindBufferMemory(device_, vertexBuffer_, deviceMemory_, 0);
	vkBindBufferMemory(device_, indexBuffer_, deviceMemory_,
					   indexBufferOffset);

	void* mapping = nullptr;
	vkMapMemory(device_, deviceMemory_, 0, VK_WHOLE_SIZE,
				0, &mapping);
	::memcpy(mapping, vertices, sizeof(vertices));

	::memcpy(static_cast<uint8_t*>(mapping) + indexBufferOffset,
			 indices, sizeof(indices));

	if (!memoryIsHostCoherent) {
		VkMappedMemoryRange mappedMemoryRange = {};
		mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mappedMemoryRange.memory = deviceMemory_;
		mappedMemoryRange.offset = 0;
		mappedMemoryRange.size = VK_WHOLE_SIZE;

		vkFlushMappedMemoryRanges(device_, 1, &mappedMemoryRange);
	}

	vkUnmapMemory(device_, deviceMemory_);
}

///////////////////////////////////////////////////////////////////////////////
void /*VulkanQuad*/ VulkanSample::CreatePipelineStateObject() {
	vertexShader_ = LoadShader(device_, BasicVertexShader, sizeof(BasicVertexShader));
	fragmentShader_ = LoadShader(device_, BasicFragmentShader, sizeof(BasicFragmentShader));

	pipelineLayout_ = CreatePipelineLayout(device_);
	VkExtent2D extent = {
		static_cast<uint32_t>(window_->GetWidth()),
		static_cast<uint32_t>(window_->GetHeight())
	};
	pipeline_ = CreatePipeline(device_, renderPass_, pipelineLayout_,
							   vertexShader_, fragmentShader_, extent);
}

#endif
