#include "App.h"

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugReportCallbackEXT *pCallback)
{
    auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackEXT callback, const VkAllocationCallbacks * pAllocator)
{
    auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    
    if (func != nullptr)
    {
        func(instance, callback, pAllocator);
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char *layerPrefix, const char *msg, void *userData)
{
    printf("Validation layer: %s\n", msg);

    return VK_FALSE;
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
    printf("App::initVulkan\n");

    createVulkanInstance();
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
        DestroyDebugReportCallbackEXT(m_vkInstance, m_debugReportCallback, nullptr);
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

    printf("Glfw extensions\n");
    for (const char *extension : extensions)
    {
        printf(" - %s\n", extension);
    }

    if (!ENABLE_VALIDATION_LAYERS)
    {
        instanceCreateInfo.enabledLayerCount = 0;
    }
    else
    {
        checkValidationLayersSupport();

        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        instanceCreateInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    }

    if (vkCreateInstance(&instanceCreateInfo, nullptr, &m_vkInstance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create vulkan instance");
    }

    if (ENABLE_VALIDATION_LAYERS)
    {
        setupDebugCallback();
    }
}

void App::checkValidationLayersSupport()
{
    uint32_t layersCount;
    vkEnumerateInstanceLayerProperties(&layersCount, nullptr);

    std::vector<VkLayerProperties> layers(layersCount);
    vkEnumerateInstanceLayerProperties(&layersCount, layers.data());

    printf("Checking for validation layers support\n");
    for (const char *layerName : VALIDATION_LAYERS)
    {
        bool layerFound = false;

        for (VkLayerProperties layerProperties : layers)
        {
            if (strcmp(layerProperties.layerName, layerName))
            {
                layerFound = true;
                break;
            }
        }

        printf(" - %s : %s\n", layerName, layerFound ? "supported" : "not supported");

        if (!layerFound)
        {
            throw std::runtime_error("Missing validation layer support");
        }
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

    if (ENABLE_VALIDATION_LAYERS)
    {
        extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
}

void App::setupDebugCallback()
{
    VkDebugReportCallbackCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = debugCallback;

    if (CreateDebugReportCallbackEXT(m_vkInstance, &createInfo, nullptr, &m_debugReportCallback) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug callback!");
    }
}