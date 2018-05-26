#include "QueueFamilies.h"

QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice &physicalDevice)
{
    QueueFamilyIndicies queueFamilyIndicies;
    
    uint32_t queueFamilyPropertyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

    for (int idx = 0; idx < queueFamilyPropertyCount; ++idx)
    {
        VkQueueFamilyProperties properties = queueFamilyProperties[idx];

        if (properties.queueCount > 0)
        {
            if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                queueFamilyIndicies.graphics = idx;                
            }
        }
    }

    return queueFamilyIndicies;
}

bool areAllQueueFamiliesFound(QueueFamilyIndicies &queueFamilyIndicies)
{
    return
        queueFamilyIndicies.graphics != -1;
}