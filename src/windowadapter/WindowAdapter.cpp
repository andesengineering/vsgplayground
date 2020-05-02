#include "WindowAdapter.h"
#include "XCBNativeAdapter.h"
#include "XlibNativeAdapter.h"
#include <xcb/xcb.h>
#include <X11/Xlib.h>

// Xlib
template <>
WindowAdapter::WindowAdapter<Display *,unsigned long>( vsg::ref_ptr<vsg::WindowTraits> traits, Display *dpy, unsigned long win ):
vsg::Window(traits, nullptr)
{
    _nativeAdapter = new XlibNativeAdapter( dpy, win );
    _init();
}

// Xcb
template <>
WindowAdapter::WindowAdapter<xcb_connection_t *,xcb_window_t>( vsg::ref_ptr<vsg::WindowTraits> traits, xcb_connection_t* connection, xcb_window_t window):
vsg::Window(traits, nullptr)
{
    _nativeAdapter = new XcbNativeAdapter( connection, window );
    _init();
}

void WindowAdapter::_init()
{
    VkSurfaceKHR surf;
    _nativeAdapter->createVulkanSurface( _instance->getInstance(), surf );

    _surface = new vsg::Surface( surf, _instance );
    initaliseDevice();

    uint32_t width = 0;
    uint32_t height = 0;
    _nativeAdapter->getFramebufferSize( width, height );

    resize();
}


bool WindowAdapter::valid() const
{
    if( _nativeAdapter )
        return _nativeAdapter->valid();

    return false;
}


bool WindowAdapter::pollEvents(vsg::Events& events)
{
    if( _nativeAdapter )
        return _nativeAdapter->pollEvents( events );
    return false;
}


bool WindowAdapter::resized() const
{
    if( _nativeAdapter )
        return _nativeAdapter->resized();
    return false;
}


void WindowAdapter::resize()
{
    if( _nativeAdapter )
    {
        uint32_t width, height;
        _nativeAdapter->getFramebufferSize( width, height );
        buildSwapchain( width, height );
    }
}
