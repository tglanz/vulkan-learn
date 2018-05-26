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

bool isPhysicalDeviceSuitable(VkPhysicalDevice &physicalDevice)
{
    QueueFamilyIndicies queueFamilyIndicies = findQueueFamilies(physicalDevice);
    return areAllQueueFamiliesFound(queueFamilyIndicies);
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
    pickPhysicalDevice();
    setupDebugCallback();

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

    if (m_isEnableValidationLayers)
    {
        DestroyDebugReportCallbackEXT(m_vkInstance, m_vkDebugReportCallback, nullptr);
    }

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

    if (!m_isEnableValidationLayers)
    {
        instanceCreateInfo.enabledLayerCount = 0;
    }
    else
    {
        validateLayersAreSupported(m_validationLayers);
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = m_validationLayers.data();
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

    std::vector<const char *> extensions;
    for (int idx = 0; idx < glfwExtensionCount; ++idx)
    {
        extensions.push_back(glfwExtensions[idx]);
    }

    if (m_isEnableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
}

void App::setupDebugCallback()
{
    if (!m_isEnableValidationLayers)
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

        if (isPhysicalDeviceSuitable(physicalDevice))
        {
            printf("  - Device suitable\n");
            m_vkPhysicalDevice = physicalDevice;
        }
    }

    if (m_vkPhysicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("Unable to find a suitable device");
    }
}