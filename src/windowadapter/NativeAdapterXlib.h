#ifndef NATIVE_ADAPTER_XLIB_DEF
#define NATIVE_ADAPTER_XLIB_DEF

#include "WindowAdapter.h"
#include "macros.h"
#include <X11/Xlib.h>

class NativeAdapterXlib: public WindowAdapter::NativeAdapter
{
    public:
        NO_DEFAULT_CONSTRUCTOR(NativeAdapterXlib)

        NativeAdapterXlib( Display *, unsigned long );

        bool valid() override;
        void createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface ) override;
        bool pollEvents( vsg::Events & ) override;
        bool resized() override;
        bool getFramebufferSize( uint32_t &width, uint32_t &height ) override;

        static vsg::Names getRequiredExtensions();

    private:
        Display *dpy = nullptr;
        unsigned long window;
};

#endif

