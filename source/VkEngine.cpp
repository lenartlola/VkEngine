#include "VkEngine.hpp"
#include "utils.hpp"
#include <vector>
#include <set>
#include <limits>
#include <algorithm>

void	VkEngine::vk_init()
{
	initWindow();
	initVulkan();
}

void	VkEngine::run()
{
	vk_loop();
	vk_destroy();
}

void	VkEngine::vk_loop()
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void	VkEngine::vk_destroy()
{
	vkDestroySurfaceKHR(instance, surface, nullptr);
	std::cout << RVOUT << "Surface has been destroyed." << std::endl;
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(logicalDevice, imageView, nullptr);
		std::cout << RVOUT << "SwapChain image view has been destroyed: " << imageView << std::endl;
	}
	vkDestroySwapchainKHR(logicalDevice, swapChain, nullptr);
	std::cout << RVOUT << "SwapChain has been destroyed." << std::endl;
	vkDestroyDevice(logicalDevice, nullptr);
	std::cout << RVOUT << "Logical device has been destroyed." << std::endl;
	vkDestroyInstance(instance, nullptr);
	std::cout << RVOUT << "Instance device has been destroyed." << std::endl;
	glfwDestroyWindow(window);
	std::cout << RVOUT << "Window device has been destroyed." << std::endl;
	glfwTerminate();
	std::cout << RVOUT << "Terminating GLFW." << std::endl;
	std::cout << GVOUT << "Good bye <3" << std::endl;
}

void	VkEngine::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //Disable window resize
	window = glfwCreateWindow(WIDTH, HEIGHT, "VkEngine", nullptr, nullptr);
}

void	VkEngine::initVulkan()
{
	createInstance();
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
	createImageView();
}

void	VkEngine::createInstance()
{
	VkApplicationInfo		appInfo{};
	VkInstanceCreateInfo	createInfo{};
	VkResult				result;

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "VkEngine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "VkEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> requiredExtensions;

	std::set<std::string > extensions = getRequiredExtensions();
	for (const auto & it : extensions)
	{
		requiredExtensions.emplace_back(it.data());
		//std::cout << GDBG << it.data() << std::endl;
	}

	createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();
	result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result == VK_SUCCESS)
	{
		std::cout << GVOUT << "Instance has been created successfully, VK_SUCCESS" << KWHT << std::endl;
		std::cout << GVOUT << createInfo.enabledExtensionCount << " extensions have been enabled" << KWHT << std::endl;
	}
	else
    {
		std::cout << result << std::endl;
        printVkResultErrorMessage(result);
        throw std::runtime_error("Failed to create instance!");
    }
}

void	VkEngine::createSurface()
{
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
	if (result != VK_SUCCESS)
	{
		printVkResultErrorMessage(result);
		throw std::runtime_error(KRED"failed to create window surface!");
	}
	std::cout << GVOUT << "Window surface has been created successfully." << KWHT << std::endl;
}

void	VkEngine::pickPhysicalDevice() {
	VkPhysicalDevice						phDevice;
	uint32_t								deviceCount;
	std::multimap<int, VkPhysicalDevice>	candidates;

	phDevice = VK_NULL_HANDLE;
	deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to enumerate physical device!");
		//TODO vk_destroy all.
	}
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // TODO find a right method to choose the right GPU when have multiple
    // One way is to create a map and give each device a score depending on their feature and properties.
    // Or we can just let the users to choose the suitable device by showing them a list of device names.

    for (const auto &device : devices) {
        if (deviceCount == 1)
		{
			phDevice = device;
			// TODO check swap chain capacity in a proper way and examine it
			//showSupportedDevices(phDevice);
			//showSwapChainCaps(detailStructure);
		}
        else
		{
            if (isDeviceSuitable(device))
			{
				phDevice = device;
                break;
            }
        }
    }
	if (phDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
		//TODO vk_destroy all.
	}
	std::cout << GVOUT << "Physical device has been created successfully " << phDevice << KWHT << std::endl;
	physicalDevice = phDevice;
}

void VkEngine::createLogicalDevice() {
	float										queuePriority;
	std::vector<VkDeviceQueueCreateInfo>		queueCreateInfos{};
	VkPhysicalDeviceFeatures					deviceFeatures{};
	VkDeviceCreateInfo							createInfo{};
	std::set<uint32_t> 							uniqueQueueFamilies;

	indices = findQueueFamily(physicalDevice);
	uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
	queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	std::cout << GVOUT << "Device extensions have been enabled successfully" << std::endl;
	std::cout << GVOUT << createInfo.enabledExtensionCount << " device extension have been enabled." << std::endl;
	createInfo.enabledLayerCount = 0;
	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
		throw std::runtime_error(KRED"failed to create logical device!");
	}
	std::cout << GVOUT << "Logical device has been created successfully: " << logicalDevice << KWHT << std::endl;
	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	std::cout << GVOUT << "Graphic queue has been retrieved successfully: " << graphicsQueue << KWHT << std::endl;
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
	std::cout << GVOUT << "Present queue has been retrieved successfully: " << presentQueue << KWHT << std::endl;
}

void	VkEngine::createSwapChain()
{
	// The number of image we want to have in the swap chain
	uint32_t 					imageCount;
	// swap chain object structure
	VkSwapchainCreateInfoKHR	createInfo{};
	VkResult					result;

	schDetails = querySwapChainSupport(physicalDevice);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(schDetails.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(schDetails.presentModes);
	VkExtent2D extent = chooseSwapExtent(schDetails.capabilities);
	imageCount = schDetails.capabilities.minImageCount;
	if (schDetails.capabilities.maxImageCount > 0 && imageCount > schDetails.capabilities.maxImageCount)
		imageCount = schDetails.capabilities.maxImageCount;
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1; // Should be changed for stereoscopic 3D application
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // We may use VK_IMAGE_USAGE_TRANSFER_DST_BIT later on

	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
	if (indices.graphicsFamily != indices.presentFamily)
	{
		// VK_SHARING_MODE_CONCURRENT: Images can be used across multiple queue families without
		// explicit ownership transfers.
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		// VK_SHARING_MODE_EXCLUSIVE: An image is owned by one queue family at a time and ownership must be explicitly
		// transferred before using it in another queue family. This option offers the best performance.
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}
	createInfo.preTransform = schDetails.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;
	result = vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain);
	if (result != VK_SUCCESS)
	{
		printVkResultErrorMessage(result);
		throw std::runtime_error(KRED"failed to create swap chain!");
	}
	std::cout << GVOUT << "SwapChain has been created successfully: " << swapChain << std::endl;
	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(logicalDevice, swapChain, &imageCount, swapChainImages.data());
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
	std::cout << GVOUT << "Swap chain images have been retrieved successfully: " << imageCount << std::endl;
	std::cout << GVOUT << "Swap chain images format: " << swapChainImageFormat << std::endl;
	std::cout << GVOUT << "Swap chain images extent width: " << swapChainExtent.width << ", height: " << swapChainExtent.height << std::endl;
}

void VkEngine::createImageView() {
	swapChainImageViews.resize(swapChainImages.size());

	for (size_t i = 0; i < swapChainImages.size(); i++)
	{
		VkResult				res;
		VkImageViewCreateInfo	createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // Could be changed to 1D or 3D
		createInfo.format = swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		res = vkCreateImageView(logicalDevice, &createInfo, nullptr, &swapChainImageViews[i]);
		if (res != VK_SUCCESS)
		{
			printVkResultErrorMessage(res);
			throw std::runtime_error(KRED"Failed to create image view");
		}
		std::cout << GVOUT << "Image view has been created successfully: " << swapChainImageViews[i] << std::endl;
	}
}

VkEngine::QueueFamilyIndices VkEngine::findQueueFamily(VkPhysicalDevice device) {
	QueueFamilyIndices	indices;
	uint32_t			queueFamilyCount;

	queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());


	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphicsFamily = i;
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (presentSupport)
			indices.presentFamily = i;
		if (indices.isComplete())
			break;
		i++;
	}
	if (indices.isComplete())
		std::cout << GVOUT << "Found queue family" << KWHT << std::endl;
	return indices;
}

VkEngine::SwapChainDetails	VkEngine::querySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainDetails	details;
	uint32_t			formatCount;
	uint32_t 			presentModeCount;

	// query surface capabilities, formats, present modes
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}
	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	return details;
}

VkExtent2D VkEngine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;
	else
	{
		int	width;
		int	height;
		glfwGetFramebufferSize(window, &width, &height);
		VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
		};
		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return actualExtent;
	}
}

//void	VkEngine::showSupportedDevices(VkPhysicalDevice device)
//{
//	// TODO this should as well be used to pick the right device
//	uint32_t extensionCount;
//
//	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
//	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
//	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
//	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
//	std::cout << "==================(Supported extensions)==================" << std::endl;
//	for (const auto& extension : availableExtensions) {
//		std::cout << GDBG << extension.extensionName << std::endl;
//		requiredExtensions.erase(extension.extensionName);
//	}
//	std::cout << "==========================================================" << std::endl;
//	std::cout << GDBG << "SwapChain capability? " << (requiredExtensions.empty() ? "Yes" : "No") << std::endl;
//}
//
//void	VkEngine::showSwapChainCaps(SwapChainDetails schDetail)
//{
//	// TODO this should as well be used to pick the right device instead of just printing
//	std::cout << "==================(SwapChain Capabilities)==================" << std::endl;
//	std::cout << "minImageCount: " << schDetail.capabilities.minImageCount << std::endl;
//	std::cout << "maxImageCount: " << schDetail.capabilities.maxImageCount << std::endl;
//	std::cout << "currentExtentHeight: " << schDetail.capabilities.currentExtent.height << std::endl;
//	std::cout << "currentExtentWidth: " << schDetail.capabilities.currentExtent.width << std::endl;
//	std::cout << "minImageExtentHeight: " << schDetail.capabilities.minImageExtent.height << std::endl;
//	std::cout << "minImageExtentWidth: " << schDetail.capabilities.minImageExtent.width << std::endl;
//	std::cout << "maxImageExtentHeight: " << schDetail.capabilities.maxImageExtent.height << std::endl;
//	std::cout << "maxImageExtentWidth: " << schDetail.capabilities.maxImageExtent.width << std::endl;
//	std::cout << "maxImageArrayLayers: " << schDetail.capabilities.maxImageArrayLayers << std::endl;
//}
