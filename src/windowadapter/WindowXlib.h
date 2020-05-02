#ifndef WINDOW_XLIB_DEF
#define WINDOW_XLIB_DEF

#include <X11/Xlib.h>
#include <vsg/core/Object.h>

struct WindowXlib: public vsg::Object
{
    // Leave dpy and window exposed.  That's all we need.
    Display *dpy = nullptr;
    Window window;

    WindowXlib(uint32_t width, uint32_t height);
    ~WindowXlib();
};

#endif
