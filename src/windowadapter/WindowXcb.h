#ifndef WINDOW_XCB_DEF
#define WINDOW_XCB_DEF

#include <xcb/xcb.h>
#include <vsg/core/Object.h>

struct WindowXcb: public vsg::Object
{
    // Leave connection and window exposed.  That's all we need.
    xcb_connection_t* connection = nullptr;
    xcb_window_t window{};

    WindowXcb(uint32_t width, uint32_t height);
    ~WindowXcb();
};

#endif
