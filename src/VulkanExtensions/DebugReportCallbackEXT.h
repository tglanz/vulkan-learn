#pragma once

#include <vulkan/vulkan.h>

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback);
void DestroyDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator);