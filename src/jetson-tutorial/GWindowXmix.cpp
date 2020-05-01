#ifdef USE_XMIX

#include <iostream>
#include "GWindowXmix.h"
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <vulkan/vulkan_xlib.h>

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


bool GWindowXmix::closePending()
{
  return false;
}

void GWindowXmix::poll()
{
  xcb_generic_event_t* event;
  while ((event = xcb_poll_for_event(_connection)))
  {
    std::cout << "." << std::flush;
  }
}

void GWindowXmix::getFramebufferSize( uint32_t &width, uint32_t &height )
{
  xcb_get_geometry_cookie_t geometry_cookie = xcb_get_geometry(_connection, _window);
  xcb_query_tree_cookie_t tree_cookie = xcb_query_tree(_connection, _window);

  xcb_get_geometry_reply_t* geometry_reply = xcb_get_geometry_reply(_connection, geometry_cookie, nullptr);
  xcb_query_tree_reply_t* tree_reply = xcb_query_tree_reply(_connection, tree_cookie, nullptr);

  if (geometry_reply)
  {
    int x = geometry_reply->x;
    int y = geometry_reply->y;
    width  = geometry_reply->width;
    height = geometry_reply->height;

    if (tree_reply)
    {
      xcb_translate_coordinates_cookie_t trans_cookie = xcb_translate_coordinates(_connection, _window, geometry_reply->root, x, y);
      xcb_translate_coordinates_reply_t* trans_reply = xcb_translate_coordinates_reply(_connection, trans_cookie, nullptr);
      
      if (trans_reply)
      {
        x = trans_reply->dst_x;
        y = trans_reply->dst_y;
        free(trans_reply);
      }
      free(tree_reply);
    }
    free(geometry_reply);
  }
}

void GWindowXmix::waitEvents()
{
  if( _connection )
    xcb_wait_for_event(_connection);
}

void GWindowXmix::createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface )
{
    VkXlibSurfaceCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext = NULL;
    createInfo.flags = 0;
    createInfo.dpy = dpy;
    createInfo.window = _window;

    VkResult err;
    err = vkCreateXlibSurfaceKHR( instance, &createInfo, NULL, &surface);

    std::cout << "Create VULKAN Surface ERROR: " << err << std::endl;
}

std::vector<const char*> GWindowXmix::getRequiredExtensions( bool )
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

GWindowXmix::GWindowXmix( uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback )
{
  int screenNum = 0;
  dpy = XOpenDisplay("");
  _connection = XGetXCBConnection(dpy);
  std::cout << "screen number: " << screenNum << std::endl;

  if (xcb_connection_has_error(_connection))
  {
    // close connection
    xcb_disconnect(_connection);
    throw 1;
  }

  const xcb_setup_t* setup = xcb_get_setup(_connection);

  std::cout << "setup: " << setup << std::endl;

  int screenCount = xcb_setup_roots_length (setup);

  std::cout << "Screen count: " << screenCount << std::endl;


  _window = xcb_generate_id(_connection);

  std::cout << "Window: " << _window << std::endl;

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

    xcb_create_window(_connection, depth, _window, parent,
        0, 0, 800, 600, 0,
        window_class,
        visual,
        value_mask,
        value_list );

  xcb_map_window(_connection, _window);

  xcb_flush(_connection);

}

GWindowXmix::~GWindowXmix()
{
  if (_connection != nullptr)
  {
    if (_window != 0) 
      xcb_destroy_window(_connection, _window); 
    xcb_flush(_connection);
    xcb_disconnect(_connection);
  }
}

#endif
