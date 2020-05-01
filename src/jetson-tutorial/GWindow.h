#ifndef GWINDOW_DEF
#define GWINDOW_DEF

#include <vulkan/vulkan.h>
#include <functional>
#include <memory>
#include <vector>

class GWindow;
using GWindowPtr = std::shared_ptr<GWindow>;

class GWindow
{
    public:

        static GWindowPtr create( uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback );

        virtual bool closePending() { return false; }

        virtual void poll() {} 

        virtual void getFramebufferSize( uint32_t &width, uint32_t &height ) {}

        virtual void waitEvents() {}

        virtual void createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface ) {}

        virtual std::vector<const char*> getRequiredExtensions(bool) {}

    protected:
        GWindow() {}
        virtual ~GWindow() {}

    private:
        std::function<void(uint32_t, uint32_t)> resizeCallback;

};
#endif
