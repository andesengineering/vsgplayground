#ifndef GWINDOW_XLIB_DEF
#define GWINDOW_XLIB_DEF

#ifdef USE_XLIB

#include "GWindow.h"

#include <X11/Xlib.h>
#include <functional>
#include <vector>

class GWindowXlib: public GWindow
{
    public:

        virtual bool closePending() override;

        virtual void poll() override;

        virtual void getFramebufferSize( uint32_t &width, uint32_t &height ) override ;

        virtual void waitEvents() override;

        virtual void createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface ) override;

        virtual std::vector<const char*> getRequiredExtensions( bool ) override;

    protected:
        GWindowXlib( uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback );
        virtual ~GWindowXlib();

    private:
        Display *dpy;
        Window win;
        bool close;
        std::function<void(uint32_t, uint32_t)> resizeCallback;

        uint32_t width, height;

};

#endif

#endif
