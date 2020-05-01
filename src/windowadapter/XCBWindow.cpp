#include "XCBWindow.h"

#ifdef USE_XCB

XCBWindow::XCBWindow( uint32_t width, uint32_t height )
{
    int screenNum = 0;
    connection = xcb_connect(nullptr, &screenNum);

    if (xcb_connection_has_error(connection))
    {
        // close connection
        xcb_disconnect(connection);
        throw 1;
    }

    const xcb_setup_t* setup = xcb_get_setup(connection);
    int screenCount = xcb_setup_roots_length (setup);

    window = xcb_generate_id(connection);

    xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(setup);

    xcb_screen_t *screen = screen_iterator.data;

	uint8_t depth = XCB_COPY_FROM_PARENT;
  	xcb_window_t parent = screen->root;
  	uint16_t border_width = 0;
  	uint16_t window_class = XCB_WINDOW_CLASS_INPUT_OUTPUT;
  	xcb_visualid_t visual = XCB_COPY_FROM_PARENT;
  	uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  	uint32_t event_mask =
		  XCB_EVENT_MASK_EXPOSURE |
		  XCB_EVENT_MASK_STRUCTURE_NOTIFY |
		  XCB_EVENT_MASK_KEY_PRESS |
		  XCB_EVENT_MASK_KEY_RELEASE |
		  XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_BUTTON_PRESS |
		  XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW |
		  XCB_EVENT_MASK_FOCUS_CHANGE |
		  XCB_EVENT_MASK_PROPERTY_CHANGE;

	uint32_t value_list[] =
        {
            screen->black_pixel,
            event_mask
        };

	xcb_create_window(connection, depth, window, parent,
			0, 0, width, height, 0,
			window_class,
			visual,
			value_mask,
			value_list );

  	xcb_map_window(connection, window);

  	xcb_flush(connection);
}

XCBWindow::~XCBWindow()
{
    if( connection != nullptr)
    {
        if (window != 0)
            xcb_destroy_window(connection, window);
        xcb_flush(connection);
        xcb_disconnect(connection);
    }
}

#endif
