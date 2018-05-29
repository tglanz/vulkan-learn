#include "App.h"

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char *layerPrefix,
    const char *msg,
    void *userData)
{
    printf("Validation layer: %s\n", msg);
    return VK_FALSE;
}

void validateLayersAreSupported(const std::vector<const char *> &layerNames)
{
    uint32_t supportedLayerCount;
    vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr);

    std::vector<VkLayerProperties> supportedLayers(supportedLayerCount);
    vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers.data());

    for (auto layerName : layerNames)
    {
        bool isFound = false;
        for (auto supportedLayer : supportedLayers)
        {
            if (strcmp(layerName, supportedLayer.layerName))
            {
                isFound = true;
                break;
            }
        }

        if (!isFound)
        {
            char buffer[254];
            sprintf(buffer, "Unsupported layer: %s", layerName);
            throw std::runtime_error(buffer);
        }
    }
}

bool isPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const std::vector<const char *> &deviceExtensions)
{
    QueueFamilyIndicies queueFamilyIndicies = findQueueFamilies(physicalDevice, surface);
    if (areAllQueueFamiliesFound(queueFamilyIndicies))
    {
        if (areAllDeviceExtensionsSupported(physicalDevice, deviceExtensions))
        {
            return true;
        }
    }

    return false;
}

void App::run()
{
    printf("App::run\n");

    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void App::initWindow()
{
    printf("App::initWindow\n");

    if (glfwInit() == GLFW_FALSE)
    {
        throw std::runtime_error("Failed glfwInit");
    }

    // Tell GLFW to not use OpenGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Not resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan App", nullptr, nullptr);
}

void App::initVulkan()
{
    printf("App::initVulkan - start\n");

    createVulkanInstance();
    createSurface();
    setupDebugCallback();
    pickPhysicalDevice();
    createLogicalDevice();

    printf("App::initVulkan - finish\n");
}

void App::mainLoop()
{
    while (glfwWindowShouldClose(m_window) == GLFW_FALSE)
    {
        glfwPollEvents();
    }
}

void App::cleanup()
{
    printf("App::cleanup\n");

    if (ENABLE_VALIDATION_LAYERS)
    {
        DestroyDebugReportCallbackEXT(m_vkInstance, m_vkDebugReportCallback, nullptr);
    }

    vkDestroyDevice(m_vkDevice, nullptr);
    vkDestroySurfaceKHR(m_vkInstance, m_vkSurfaceKHR, nullptr);
    vkDestroyInstance(m_vkInstance, nullptr);

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void App::createVulkanInstance()
{
    VkApplicationInfo applicationInfo = {};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = "my application";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName = "no engine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;

    std::vector<const char *> extensions = getRequiredExtensions();
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

    if (!ENABLE_VALIDATION_LAYERS)
    {
        instanceCreateInfo.enabledLayerCount = 0;
    }
    else
    {
        validateLayersAreSupported(VALIDATION_LAYERS);
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        instanceCreateInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }

    if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_vkInstance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create vulkan instance");
    }
}

std::vector<const char *> App::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    printf("Required extensions\n");
    std::vector<const char *> extensions;
    for (int idx = 0; idx < glfwExtensionCount; ++idx)
    {
        printf(" - %s\n", glfwExtensions[idx]);
        extensions.push_back(glfwExtensions[idx]);
    }

    if (ENABLE_VALIDATION_LAYERS)
    {
        printf(" - %s\n", VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
}

void App::setupDebugCallback()
{
    if (!ENABLE_VALIDATION_LAYERS)
    {
        return;
    }

    VkDebugReportCallbackCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = debugCallback;

    if (CreateDebugReportCallbackEXT(m_vkInstance, &createInfo, nullptr, &m_vkDebugReportCallback) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to set up debug callback");
    }
}

void App::pickPhysicalDevice()
{
    printf("App::pickPhysicalDevice - start\n");

    uint32_t physicalDeviceCount;
    vkEnumeratePhysicalDevices(m_vkInstance, &physicalDeviceCount, nullptr);

    if (physicalDeviceCount == 0)
    {
        throw std::runtime_error("No physical devices");
    }

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(m_vkInstance, &physicalDeviceCount, physicalDevices.data());

    printf("Physical devices found\n");
    for (VkPhysicalDevice physicalDevice : physicalDevices)
    {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
        printf(" - %s\n", physicalDeviceProperties.deviceName);

        if (isPhysicalDeviceSuitable(physicalDevice, m_vkSurfaceKHR, DEVICE_EXTENSIONS))
        {
            printf("  - Device suitable\n");
            m_vkPhysicalDevice = physicalDevice;
        }
    }

    if (m_vkPhysicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Unable to find a suitable device");
    }

    printf("App::pickPhysicalDevice - finish\n");
}

void App::createLogicalDevice()
{
    printf("App::createLogicalDevice - start\n");

    QueueFamilyIndicies queueFamilyInicies = findQueueFamilies(m_vkPhysicalDevice, m_vkSurfaceKHR);

    float queuePriorities = 1.0f;
    
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilyIndicies = { queueFamilyInicies.graphics, queueFamilyInicies.present };
    for (int uniqueQueueFamilyIndex : uniqueQueueFamilyIndicies)
    {
        VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.queueCount = 1;
        deviceQueueCreateInfo.queueFamilyIndex = uniqueQueueFamilyIndex;
        deviceQueueCreateInfo.pQueuePriorities = &queuePriorities;
        queueCreateInfos.push_back(deviceQueueCreateInfo);
    }

    VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    deviceCreateInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();
    
    if (!ENABLE_VALIDATION_LAYERS)
    {
        deviceCreateInfo.enabledLayerCount = 0;
    }
    else
    {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        deviceCreateInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }

    VkResult createDeviceResult = vkCreateDevice(m_vkPhysicalDevice, &deviceCreateInfo, nullptr, &m_vkDevice);
    if (createDeviceResult != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create logical device");
    }

    printf("Getting device queues\n");
    vkGetDeviceQueue(m_vkDevice, queueFamilyInicies.graphics, 0, &m_vkGraphicsQueue);
    vkGetDeviceQueue(m_vkDevice, queueFamilyInicies.present, 0, &m_vkPresentQueue);

    printf("App::createLogicalDevice - finish\n");
}

void App::createSurface()
{
    printf("App::createSurface - start\n");

    if (glfwCreateWindowSurface(m_vkInstance, m_window, nullptr, &m_vkSurfaceKHR) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create window surface");
    }

    printf("App::createSurface - finish\n");
}