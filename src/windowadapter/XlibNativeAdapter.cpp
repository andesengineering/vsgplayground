#include "XlibNativeAdapter.h"

#include <vulkan/vulkan_xlib.h>

XlibNativeAdapter::XlibNativeAdapter( Display *_dpy, Window _win ):
    dpy(_dpy)
  , window(_win)
{}

bool XlibNativeAdapter::valid()
{
    return dpy != nullptr && window != 0;
}

void XlibNativeAdapter::createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface )
{
    VkXlibSurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.dpy = dpy;
    createInfo.window = window;

    VkResult err;
    err = vkCreateXlibSurfaceKHR( instance, &createInfo, NULL, &surface);
}

bool XlibNativeAdapter::pollEvents( vsg::Events & )
{
    return false; 
}

bool XlibNativeAdapter::resized()
{
    return false;
}

bool XlibNativeAdapter::getFramebufferSize( uint32_t &width, uint32_t &height )
{
    if( dpy == nullptr )
        return false;

    XWindowAttributes watt;
    XGetWindowAttributes( dpy, window, &watt );
    width = watt.width;
    height = watt.height;
    return true;
}

