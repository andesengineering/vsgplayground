#include "XlibWindow.h"

#include <X11/Xutil.h>

#ifdef USE_XLIB

XlibWindow::XlibWindow( uint32_t width, uint32_t height )
{
    dpy = XOpenDisplay("");

    XInitThreads();

    long visualMask = VisualScreenMask;
    int numberOfVisuals = 0;
    XVisualInfo vInfoTemplate={};
    vInfoTemplate.screen = DefaultScreen(dpy);

    XVisualInfo *visualInfo = XGetVisualInfo(dpy, visualMask, &vInfoTemplate, &numberOfVisuals);

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
    windowAttributes.event_mask = emask; //KeyPressMask | KeyReleaseMask | StructureNotifyMask | ExposureMask;

    window = XCreateWindow( dpy, RootWindow(dpy, vInfoTemplate.screen), 0, 0,
                width, height, 0, visualInfo->depth, InputOutput,
                visualInfo->visual,
                CWBackPixel | CWBorderPixel | CWEventMask | CWColormap, &windowAttributes);

    XSelectInput( dpy, window, windowAttributes.event_mask );
    XMapWindow(dpy, window);
    XFlush(dpy);
    XSync(dpy,0);
}

XlibWindow::~XlibWindow()
{
    if( dpy != nullptr)
    {
        if (window != 0)
            XDestroyWindow(dpy, window );
        XFlush(dpy);
        XCloseDisplay(dpy);
    }
}

#endif
