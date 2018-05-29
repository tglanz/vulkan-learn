#pragma once

#include <vulkan/vulkan.h>

#include <vector>

struct QueueFamilyIndicies
{
    int graphics = -1;
    int present = -1;
};

QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR vkSurface);
bool areAllQueueFamiliesFound(QueueFamilyIndicies &queueFamilyIndicies);