#include "NativeAdapterXlib.h"

#include <vulkan/vulkan_xlib.h>

// Xlib
template <>
WindowAdapter::WindowAdapter<Display **,unsigned long>( vsg::ref_ptr<vsg::WindowTraits> traits, Display **dpy, unsigned long win ):
vsg::Window(traits, nullptr)
{
    _nativeAdapter = new NativeAdapterXlib( *dpy, win );
    _init();
}

NativeAdapterXlib::NativeAdapterXlib( Display *&_dpy, Window _win ):
    dpy(_dpy)
  , window(_win)
{}

bool NativeAdapterXlib::valid()
{
    if( dpy == nullptr )
        return false;

    /* * this is called a lot, so could be very expensive
    XWindowAttributes watt;
    Status s = XGetWindowAttributes( dpy, window, &watt );
    return s == True;
    */
    return true;
}

void NativeAdapterXlib::createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface )
{
    VkXlibSurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.dpy = dpy;
    createInfo.window = window;

    /*VkResult err;
    err =*/ vkCreateXlibSurfaceKHR( instance, &createInfo, NULL, &surface);
}

bool NativeAdapterXlib::pollEvents( vsg::Events & )
{
    return false; 
}

bool NativeAdapterXlib::resized()
{
    return false;
}

bool NativeAdapterXlib::getFramebufferSize( uint32_t &width, uint32_t &height )
{
    if( dpy == nullptr )
        return false;

    XWindowAttributes watt;
    XGetWindowAttributes( dpy, window, &watt );
    width = watt.width;
    height = watt.height;
    return true;
}

