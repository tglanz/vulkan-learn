#pragma once

#include <vulkan/vulkan.h>

#include <vector>

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails queySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

