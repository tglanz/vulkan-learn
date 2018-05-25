#pragma once

#include <stdexcept>
#include <vector>
#include <cstdio>
#include <cstring>

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

class App
{
public:
  void run();

private:
  const int WIDTH = 800;
  const int HEIGHT = 600;
  const bool ENABLE_VALIDATION_LAYERS = true;

  const std::vector<const char *> VALIDATION_LAYERS = {
      "VK_LAYER_LUNARG_standard_validation"};

  GLFWwindow *m_window;
  VkInstance m_vkInstance;
  VkDebugReportCallbackEXT m_debugReportCallback;

  void initWindow();
  void initVulkan();
  void mainLoop();
  void cleanup();

  void createVulkanInstance();
  void checkValidationLayersSupport();
  std::vector<const char *> getRequiredExtensions();
  void setupDebugCallback();
};