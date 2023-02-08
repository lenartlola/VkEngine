//
// Created by lenart on 2/4/23.
//

#include <limits>
#include "utils.hpp"

void    printVkResultErrorMessage(VkResult	result)
{
    switch (result) {
        case VK_NOT_READY:
            std::cout << KRED << "VK_NOT_READY" << KWHT << std::endl;
            break;
        case VK_TIMEOUT:
            std::cout << KRED << "VK_TIMEOUT" << KWHT << std::endl;
            break;
        case VK_EVENT_SET:
            std::cout << KRED << "VK_EVENT_SET" << KWHT << std::endl;
            break;
        case VK_EVENT_RESET:
            std::cout << KRED << "VK_EVENT_RESET" << KWHT << std::endl;
            break;
        case VK_INCOMPLETE:
            std::cout << KRED << "VK_INCOMPLETE" << KWHT << std::endl;
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            std::cout << KRED << "VK_ERROR_OUT_OF_HOST_MEMORY" << KWHT << std::endl;
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            std::cout << KRED << "VK_ERROR_OUT_OF_DEVICE_MEMORY" << KWHT << std::endl;
            break;
        case VK_ERROR_INITIALIZATION_FAILED:
            std::cout << KRED << "VK_ERROR_INITIALIZATION_FAILED" << KWHT << std::endl;
            break;
        case VK_ERROR_DEVICE_LOST:
            std::cout << KRED << "VK_ERROR_DEVICE_LOST" << KWHT << std::endl;
            break;
        case VK_ERROR_MEMORY_MAP_FAILED:
            std::cout << KRED << "VK_ERROR_MEMORY_MAP_FAILED" << KWHT << std::endl;
            break;
        case VK_ERROR_LAYER_NOT_PRESENT:
            std::cout << KRED << "VK_ERROR_LAYER_NOT_PRESENT" << KWHT << std::endl;
            break;
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            std::cout << KRED << "VK_ERROR_EXTENSION_NOT_PRESENT" << KWHT << std::endl;
            break;
        case VK_ERROR_FEATURE_NOT_PRESENT:
            std::cout << KRED << "VK_ERROR_FEATURE_NOT_PRESENT" << KWHT << std::endl;
            break;
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            std::cout << KRED << "VK_ERROR_INCOMPATIBLE_DRIVER" << KWHT << std::endl;
            break;
        case VK_ERROR_TOO_MANY_OBJECTS:
            std::cout << KRED << "VK_ERROR_TOO_MANY_OBJECTS" << KWHT << std::endl;
            break;
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            std::cout << KRED << "VK_ERROR_FORMAT_NOT_SUPPORTED" << KWHT << std::endl;
            break;
        case VK_ERROR_FRAGMENTED_POOL:
            std::cout << KRED << "VK_ERROR_FRAGMENTED_POOL" << KWHT << std::endl;
            break;
        case VK_ERROR_UNKNOWN:
            std::cout << KRED << "VK_ERROR_UNKNOWN" << KWHT << std::endl;
            break;
        default:
            std::cout << KRED << "VK_UNDEFINED_ERROR " << result << KWHT << std::endl;
            break;
    }
}

void	showProps(VkPhysicalDeviceProperties const &deviceProperties)
{
    std::cout << KGRN << "Device properties: " << KWHT << std::endl;
    std::cout << "Type: " << deviceProperties.deviceType << std::endl;
    std::cout << KBLU << "LIMITS:" << KWHT << std::endl;
    std::cout << KCYN;
    std::cout << "\tBuffer image granularity: " << deviceProperties.limits.bufferImageGranularity << std::endl;
    std::cout << "\tmax Geometry Shader Invocations: " << deviceProperties.limits.maxGeometryShaderInvocations << std::endl;
    std::cout << KWHT;
    std::cout << "ID: " << deviceProperties.deviceID << std::endl;
    std::cout << "Name: " << deviceProperties.deviceName << std::endl;
    std::cout << "api version: " << deviceProperties.apiVersion << std::endl;
    std::cout << "driver version: " << deviceProperties.driverVersion << std::endl;
    std::cout << "pipeline cache UUID: " << deviceProperties.pipelineCacheUUID << std::endl;
    std::cout << "Sparce properties" << std::endl;
    std::cout << "Vendof ID: " << deviceProperties.vendorID << std::endl;
}

std::set<std::string> getRequiredExtensions() {
	VkResult result;

	uint32_t count = 0;
	result = vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
	if (result != VK_SUCCESS) {
		printVkResultErrorMessage(result);
		throw std::runtime_error("Failed to create instance!");
	}

	std::vector<VkExtensionProperties> extensionProperties(count);

	// Get the extensions
	result = vkEnumerateInstanceExtensionProperties(nullptr, &count, extensionProperties.data());
	if (result != VK_SUCCESS) {
		printVkResultErrorMessage(result);
		throw std::runtime_error("Failed to create instance!");
	}

	std::set<std::string> extensions;
	for (auto & extension : extensionProperties) {
		extensions.insert(extension.extensionName);
	}

	return extensions;
}

bool	isDeviceSuitable(VkPhysicalDevice device) {
	// TODO this is an important function, it should be handled carefully
	VkPhysicalDeviceProperties	deviceProperties;
	VkPhysicalDeviceFeatures	deviceFeatures;

	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	showProps(deviceProperties);

//	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
}

VkSurfaceFormatKHR	chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormat;
	}
	return availableFormats[0];
}

VkPresentModeKHR	chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			return availablePresentMode;
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

