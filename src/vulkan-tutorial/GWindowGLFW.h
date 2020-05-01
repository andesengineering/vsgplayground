#ifndef GWINDOW_GLFW_DEF
#define GWINDOW_GLFW_DEF

#ifdef USE_GLFW

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "GWindow.h"

#include <functional>
#include <vector>

class GWindowGLFW: public GWindow
{
    public:

        virtual bool closePending() override;

        void poll();

        virtual void getFramebufferSize( uint32_t &width, uint32_t &height ) override ;

        virtual void waitEvents() override;

        virtual void createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface ) override;

        virtual std::vector<const char*> getRequiredExtensions( bool ) override;

    protected:
        GWindowGLFW( uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback );
        virtual ~GWindowGLFW();

    private:
        GLFWwindow *window;
        std::function<void(uint32_t, uint32_t)> resizeCallback;

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

#endif

#endif
