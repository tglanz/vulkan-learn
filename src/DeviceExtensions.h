#pragma once

#include <vulkan/vulkan.h>

#include <cstring>
#include <vector>

bool areAllDeviceExtensionsSupported(VkPhysicalDevice physicalDevice, const std::vector<const char *> &extensionNames);