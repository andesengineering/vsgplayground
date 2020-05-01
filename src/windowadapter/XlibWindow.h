#ifndef XLIB_WINDOW_DEF
#define XLIB_WINDOW_DEF

#include <X11/Xlib.h>
#include <vsg/core/Object.h>

struct XlibWindow: public vsg::Object
{
    // Leave dpy and window exposed.  That's all we need.
    Display *dpy = nullptr;
    Window window;

    XlibWindow(uint32_t width, uint32_t height);
    ~XlibWindow();
};

#endif
