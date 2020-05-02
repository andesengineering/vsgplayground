#ifndef NATIVE_ADAPTER_XCB_DEF
#define NATIVE_ADAPTER_XCB_DEF

#include "WindowAdapter.h"
#include "macros.h"
#include <xcb/xcb.h>

class NativeAdapterXcb: public WindowAdapter::NativeAdapter
{
    public:
        NO_DEFAULT_CONSTRUCTOR(NativeAdapterXcb)

        NativeAdapterXcb( xcb_connection_t* connection, xcb_window_t window );

        bool valid() override;
        void createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface ) override;
        bool pollEvents( vsg::Events & ) override;
        bool resized() override;
        bool getFramebufferSize( uint32_t &width, uint32_t &height ) override;

        static vsg::Names getRequiredExtensions();

    private:
        xcb_connection_t *_connection;
        xcb_window_t _window;
};

#endif
