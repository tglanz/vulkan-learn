#pragma once

#include <vulkan/vulkan.h>

#include <vector>

struct QueueFamilyIndicies
{
    uint32_t graphics = -1;
};

QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice &physicalDevice);
bool areAllQueueFamiliesFound(QueueFamilyIndicies &queueFamilyIndicies);