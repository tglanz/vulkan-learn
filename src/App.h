#pragma once

#include <stdexcept>
#include <vector>
#include <set>
#include <cstdio>
#include <cstring>

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include "VulkanExtensions/DebugReportCallbackEXT.h"
#include "QueueFamilies.h"
#include "DeviceExtensions.h"

class App
{
public:
  void run();

private:
  /* Constants */
  
  const int WIDTH = 800;
  const int HEIGHT = 600;
  const float MAX_QUEUE_PRIORITY = 1.0f;

  const bool ENABLE_VALIDATION_LAYERS = true;
  const std::vector<const char *> VALIDATION_LAYERS = {
    "VK_LAYER_LUNARG_standard_validation"
  };

  const std::vector<const char *> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

  /* Members */

  GLFWwindow *m_window;
  VkInstance m_vkInstance = VK_NULL_HANDLE;
  VkDebugReportCallbackEXT m_vkDebugReportCallback = VK_NULL_HANDLE;
  VkPhysicalDevice m_vkPhysicalDevice = VK_NULL_HANDLE;
  VkDevice m_vkDevice = VK_NULL_HANDLE;
  VkQueue m_vkGraphicsQueue = VK_NULL_HANDLE;
  VkQueue m_vkPresentQueue = VK_NULL_HANDLE;
  VkSurfaceKHR m_vkSurfaceKHR = VK_NULL_HANDLE;

  /* Methods */

  void initWindow();
  void initVulkan();
  void mainLoop();
  void cleanup();

  void createVulkanInstance();
  std::vector<const char *> getRequiredExtensions();
  void setupDebugCallback();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSurface();
};