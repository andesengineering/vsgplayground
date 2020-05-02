#include "NativeAdapterXlib.h"

#include <vulkan/vulkan_xlib.h>
#include <vsg/vk/Extensions.h>
#include <iostream>


// Xlib specific WindowAdapter constructor
template <>
WindowAdapter::WindowAdapter<Display *,unsigned long>( vsg::ref_ptr<vsg::WindowTraits> traits, vsg::AllocationCallbacks* allocator, Display *dpy, unsigned long win ):
    vsg::Window(traits, NativeAdapterXlib::getRequiredExtensions(), allocator)
{
    _nativeAdapter = new NativeAdapterXlib( dpy, win );
    _init();
}

vsg::Names NativeAdapterXlib::getRequiredExtensions()
{
    vsg::Names requiredExtensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_XLIB_SURFACE_EXTENSION_NAME};
    if (!vsg::isExtensionListSupported(requiredExtensions))
    {
        std::cerr << "Error: NativeAdapterXlib::getRequiredExtensions(...) unable to create window, "
                    "VK_KHR_SURFACE_EXTENSION_NAME or VK_KHR_XLIB_SURFACE_EXTENSION_NAME not supported." << std::endl;
        return vsg::Names();
    }

   return requiredExtensions;
}

NativeAdapterXlib::NativeAdapterXlib( Display *_dpy, Window _win ):
    dpy(_dpy)
  , window(_win)
{}

bool NativeAdapterXlib::valid()
{
    return dpy != nullptr && window != 0;
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

