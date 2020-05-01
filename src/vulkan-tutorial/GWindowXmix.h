#ifndef GWINDOW_XMIX_DEF
#define GWINDOW_XMIX_DEF

#ifdef USE_XMIX

#include "GWindow.h"

#include <functional>
#include <vector>

#include <X11/Xlib.h>
#include <xcb/xcb.h>

class GWindowXmix: public GWindow
{
    public:

        virtual bool closePending() override;

        void poll();

        virtual void getFramebufferSize( uint32_t &width, uint32_t &height ) override ;

        virtual void waitEvents() override;

        virtual void createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface ) override;

        virtual std::vector<const char*> getRequiredExtensions( bool ) override;

    protected:
        GWindowXmix( uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback );
        virtual ~GWindowXmix();

    private:

        std::function<void(uint32_t, uint32_t)> resizeCallback;

        Display *dpy = nullptr;
        xcb_connection_t* _connection = nullptr;
        xcb_window_t _window{};

};

#endif

#endif
