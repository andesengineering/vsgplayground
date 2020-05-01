#ifndef XLIB_NATIVE_ADAPTER_DEF
#define XLIB_NATIVE_ADAPTER_DEF

#include "WindowAdapter.h"
#include "macros.h"
#include <X11/Xlib.h>

class XlibNativeAdapter: public WindowAdapter::NativeAdapter
{
    public:
        NO_DEFAULT_CONSTRUCTOR(XlibNativeAdapter)

        XlibNativeAdapter( Display *, unsigned long );

        bool valid() override;
        void createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface ) override;
        bool pollEvents( vsg::Events & ) override;
        bool resized() override;
        bool getFramebufferSize( uint32_t &width, uint32_t &height ) override;

    private:
        Display *dpy = nullptr;
        unsigned long window;
};

#endif

