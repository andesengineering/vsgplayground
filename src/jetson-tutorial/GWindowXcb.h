#ifndef GWINDOW_GFX_DEF
#define GWINDOW_GFX_DEF

#ifdef USE_XCB

#include "GWindow.h"

#include <functional>
#include <vector>

#include <xcb/xcb.h>

class GWindowXcb: public GWindow
{
    public:

        virtual bool closePending() override;

        void poll();

        virtual void getFramebufferSize( uint32_t &width, uint32_t &height ) override ;

        virtual void waitEvents() override;

        virtual void createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface ) override;

        virtual std::vector<const char*> getRequiredExtensions( bool ) override;

    protected:
        GWindowXcb( uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback );
        virtual ~GWindowXcb();

    private:

        std::function<void(uint32_t, uint32_t)> resizeCallback;

        xcb_connection_t* _connection = nullptr;
        xcb_window_t _window{};

};

#endif

#endif
