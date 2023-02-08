#ifndef HELLO_HPP
# define HELLO_HPP

#include <cstdint>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <map>
#include <optional>
#include <set>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*>validationLayers = {
	"VK_LAYER_KHrONOS_validation"
};

#ifndef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif //NDEBUG

class VkEngine {
public:
	void	vk_init();
	void	vk_loop();
	void	vk_destroy();
private:
	struct QueueFamilyIndices {
		// std::optional is a wrapper that contains no value until we assign something to it.
		std::optional<uint32_t>	graphicsFamily;
		std::optional<uint32_t>	presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainDetails {
		VkSurfaceCapabilitiesKHR			capabilities;
		std::vector<VkSurfaceFormatKHR>		formats;
		std::vector<VkPresentModeKHR>		presentModes;
	};

	const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	// Vars
	GLFWwindow					*window = nullptr;
	VkInstance					instance;
	VkPhysicalDevice			physicalDevice;
	QueueFamilyIndices			indices;
	VkDevice 					logicalDevice;
	VkQueue						graphicsQueue;
	VkSurfaceKHR 				surface;
	VkQueue 					presentQueue;
	SwapChainDetails			schDetails;
	VkSwapchainKHR				swapChain;
	std::vector<VkImage>		swapChainImages;
	std::vector<VkImageView>	swapChainImageViews;
	VkFormat					swapChainImageFormat;
	VkExtent2D					swapChainExtent;

	// Prototypes
	void	initWindow();
	void	initVulkan();
	// https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkApplicationInfo.html
	void	createInstance();
	void 	pickPhysicalDevice();
	void	createLogicalDevice();
	void	createSurface();

	QueueFamilyIndices	findQueueFamily(VkPhysicalDevice device);

//	void	showSupportedDevices(VkPhysicalDevice device);
//	void	showSwapChainCaps(SwapChainDetails);
	void	createSwapChain();
	void	createImageView();

	SwapChainDetails	querySwapChainSupport(VkPhysicalDevice device);
//	VkSurfaceFormatKHR	chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
//	VkPresentModeKHR	chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D			chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void run();
};

#endif //HELLO_HPP
