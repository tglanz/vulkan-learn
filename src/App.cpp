#include "App.h"

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
        printf("Failed glfwInit\n");
        return;
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

    glfwDestroyWindow(m_window);
    glfwTerminate();
}