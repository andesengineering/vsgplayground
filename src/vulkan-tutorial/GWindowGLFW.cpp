#ifdef USE_GLFW
#include "GWindowGLFW.h"
#include <iostream>

GWindowGLFW::GWindowGLFW( uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback ):
    resizeCallback(callback)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);

    glfwSetWindowUserPointer(window, this );

    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

GWindowGLFW::~GWindowGLFW()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool GWindowGLFW::closePending()
{
    return glfwWindowShouldClose(window);
}

void GWindowGLFW::poll()
{
    glfwPollEvents();
}

void GWindowGLFW::getFramebufferSize( uint32_t &width, uint32_t &height )
{
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    width = static_cast<uint32_t>(w);
    height = static_cast<uint32_t>(h);
}

void GWindowGLFW::waitEvents()
{
    glfwWaitEvents();
}

void GWindowGLFW::createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface )
{
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

void GWindowGLFW::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto ws = reinterpret_cast<GWindowGLFW*>(glfwGetWindowUserPointer(window));
    if( ws->resizeCallback )
        ws->resizeCallback( width, height );
}


std::vector<const char*> GWindowGLFW::getRequiredExtensions( bool enableValidationLayers )
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    std::cout << "GLFW extensions: " << std::endl;
    for( auto &e: extensions )
        std::cout << "   ... " << e << std::endl;

    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

#endif
