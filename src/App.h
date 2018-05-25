#pragma once

#include <stdexcept>
#include <cstdio>

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

class App
{
  public:
    void run();

  private:
    const int WIDTH = 800;
    const int HEIGHT = 600;

    GLFWwindow * m_window;
    VkInstance m_vkInstance;

    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    void createVulkanInstance();
};