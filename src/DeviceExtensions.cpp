#include "DeviceExtensions.h"

bool areAllDeviceExtensionsSupported(VkPhysicalDevice physicalDevice, const std::vector<const char *> &extensionNames)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    for (const char * extensionName : extensionNames)
    {
        bool didFound = false;

        for (VkExtensionProperties extensionProperties : availableExtensions)
        {
            if (strcmp(extensionName, extensionProperties.extensionName))
            {
                didFound = true;
                break;
            }   
        }

        if (!didFound)
        {
            return false;
        }
    }

    return true;
}