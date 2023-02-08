//
// Created by lola on 2/4/23.
//

#ifndef SCOP_UTILS_HPP
#define SCOP_UTILS_HPP

#include "VkEngine.hpp"

// Colors
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

#define RDBG "\x1B[31m[DEBUG] \x1B[37m"
#define GDBG "\x1B[32m[DEBUG] \x1B[37m"
#define GVOUT "\x1B[32m[VkEngine] \x1B[37m"
#define RVOUT "\x1B[31m[VkEngine] \x1B[37m"

void    printVkResultErrorMessage(VkResult	result);
void	showProps(VkPhysicalDeviceProperties const &deviceProperties);

bool	isDeviceSuitable(VkPhysicalDevice device);

std::set<std::string>	getRequiredExtensions();

VkSurfaceFormatKHR	chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
VkPresentModeKHR	chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);


#endif //SCOP_UTILS_HPP
