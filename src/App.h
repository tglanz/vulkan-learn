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

class App
{
public:
  void run();

private:
  /* Constants */
  
  const int WIDTH = 800;
  const int HEIGHT = 600;
  const float MAX_QUEUE_PRIORITY = 1.0f;

  const bool m_isEnableValidationLayers = true;

  const std::vector<const char *> m_validationLayers = {
      "VK_LAYER_LUNARG_standard_validation"
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