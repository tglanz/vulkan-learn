#pragma once

#include <stdexcept>
#include <vector>
#include <cstdio>
#include <cstring>

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include "VulkanExtensions/DebugReportCallbackEXT.h"

class App
{
public:
  void run();

private:
  const int WIDTH = 800;
  const int HEIGHT = 600;

  const bool m_isEnableValidationLayers = true;

  const std::vector<const char *> m_validationLayers = {
      "VK_LAYER_LUNARG_standard_validation"
  };

  GLFWwindow *m_window;
  VkInstance m_vkInstance;
  VkDebugReportCallbackEXT m_vkDebugReportCallback;

  void initWindow();
  void initVulkan();
  void mainLoop();
  void cleanup();

  void createVulkanInstance();
  std::vector<const char *> getRequiredExtensions();
  void setupDebugCallback();
};