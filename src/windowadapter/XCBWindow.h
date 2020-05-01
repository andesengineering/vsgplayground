#ifndef XCB_WINDOW_DEF
#define XCB_WINDOW_DEF

#include <xcb/xcb.h>
#include <vsg/core/Object.h>

struct XCBWindow: public vsg::Object
{
    // Leave connection and window exposed.  That's all we need.
    xcb_connection_t* connection = nullptr;
    xcb_window_t window{};

    XCBWindow(uint32_t width, uint32_t height);
    ~XCBWindow();
};

#endif
