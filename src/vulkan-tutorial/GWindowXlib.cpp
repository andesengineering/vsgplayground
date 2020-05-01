#ifdef USE_XLIB
#include "GWindowXlib.h"
#include <vulkan/vulkan_xlib.h>
#include <X11/Xutil.h>

#include <vulkan/vulkan_xlib.h>

#include <iostream>

namespace 
{
  using Names = std::vector<const char*>;
  using ExtensionProperties = std::vector<VkExtensionProperties>;

  ExtensionProperties getExtensionProperties( const char *pLayerName=nullptr )
  {
    uint32_t extCount = 0;
    VkResult err = vkEnumerateInstanceExtensionProperties(pLayerName, &extCount, nullptr);
    if (err)
    {
        std::cout << "Error: vsg::getExtensionPropertiesCount(...) failed, could not get extension count from vkEnumerateInstanceExtensionProperties." << std::endl;
        return ExtensionProperties();
    }

    ExtensionProperties extensionProperties(extCount);
    err = vkEnumerateInstanceExtensionProperties(pLayerName, &extCount, extensionProperties.data());
    if (err)
    {
        std::cout << "Error: vsg::getExtensionProperties(...) failed, could not get extension properties from vkEnumerateInstanceExtensionProperties." << std::endl;
        return ExtensionProperties();
    }
    return extensionProperties;

  }
}

GWindowXlib::GWindowXlib( uint32_t _width, uint32_t _height, const std::function<void(uint32_t, uint32_t)> &callback ):
    width(_width), height(_height),
    resizeCallback(callback),
    close(false)
{
    dpy = XOpenDisplay("");

    XInitThreads();

    long visualMask = VisualScreenMask;
    int numberOfVisuals = 0;
    XVisualInfo vInfoTemplate={};
    vInfoTemplate.screen = DefaultScreen(dpy);

    XVisualInfo *visualInfo = XGetVisualInfo(dpy, visualMask, &vInfoTemplate, &numberOfVisuals);

    std::cout << "width x height: " << width << " x " << height << std::endl;
    std::cout << "NUMBER OF VISUALS: " << numberOfVisuals << std::endl;

    Colormap colormap = XCreateColormap( dpy, RootWindow(dpy, vInfoTemplate.screen), visualInfo->visual, AllocNone);

    unsigned int emask =
         KeyPressMask            |
         KeyReleaseMask          |
         EnterWindowMask         |
         LeaveWindowMask         |
         PointerMotionMask       |
         ExposureMask            |
         StructureNotifyMask     |
         ButtonPressMask         |
         ButtonReleaseMask;

    XSetWindowAttributes windowAttributes={};

    windowAttributes.colormap = colormap;
    windowAttributes.background_pixel = 0xFFFFFFFF;
    windowAttributes.border_pixel = 0;
    windowAttributes.event_mask = KeyPressMask | KeyReleaseMask | StructureNotifyMask | ExposureMask;

    win = XCreateWindow( dpy, RootWindow(dpy, vInfoTemplate.screen), 0, 0,
                width, height, 0, visualInfo->depth, InputOutput,
                visualInfo->visual,
                CWBackPixel | CWBorderPixel | CWEventMask | CWColormap, &windowAttributes);

    XSelectInput( dpy, win, windowAttributes.event_mask );
    //XSelectInput(demo->display, demo->xlib_window, ExposureMask | KeyPressMask);
    XMapWindow(dpy, win);
    XFlush(dpy);
    XSync(dpy,0);
    //demo->xlib_wm_delete_window =
    //        XInternAtom(demo->display, "WM_DELETE_WINDOW", False);

    //win = XCreateSimpleWindow( dpy, DefaultRootWindow(dpy), 0, 0, _width, _height, 0, 0, 0 );
    //XMapWindow( dpy, win );

    std::cout << "End of ctor" << std::endl;
}

GWindowXlib::~GWindowXlib()
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

bool GWindowXlib::closePending()
{
    return close;
}

void GWindowXlib::poll()
{
    XSync(dpy,0);
    while( XPending(dpy) )
    {
        XEvent ev;
        XNextEvent( dpy, &ev );

        switch( ev.type )
        {
            case MapNotify:
            case ConfigureNotify:
                {
                    XWindowAttributes watt;
                    XGetWindowAttributes( dpy, win, &watt );
                    width = watt.width;
                    height = watt.height;
                    if( resizeCallback )
                        resizeCallback( width, height );
                }
                break;
            
            case KeyPress:
                {
                    KeySym ks = XLookupKeysym( &ev.xkey, 0 );
                    switch( ks )
                    {
                        case 0xFF1B:
                            close = true;
                            break;
                    }
                }
                break;
        }
    }
}

void GWindowXlib::getFramebufferSize( uint32_t &_width, uint32_t &_height )
{
    _width = width;
    _height = height;
}

void GWindowXlib::waitEvents()
{
    XEvent ev;
    XNextEvent( dpy, &ev );
}

void GWindowXlib::createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface )
{
    VkXlibSurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.dpy = dpy;
    createInfo.window = win;

    VkResult err;
    err = vkCreateXlibSurfaceKHR( instance, &createInfo, NULL, &surface);

    std::cout << "Create VULKAN Surface ERROR: " << err << std::endl;
}

std::vector<const char*> GWindowXlib::getRequiredExtensions( bool )
{
  //Names requiredExtensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_XCB_SURFACE_EXTENSION_NAME};
  Names requiredExtensions = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_XLIB_SURFACE_EXTENSION_NAME };


  std::vector<const char*> ret;
  ExtensionProperties props = getExtensionProperties();
  std::cout << "Extensions: (" << props.size() << ")" << std::endl;
  for( auto &p: props )
  {
    std::cout << "    " << p.extensionName << std::endl;
    for( auto &e: requiredExtensions )
      if( std::string(p.extensionName) == std::string(e) )
        ret.push_back( e );
  }
  return ret;
}

#endif
