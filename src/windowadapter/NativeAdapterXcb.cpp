#include "NativeAdapterXcb.h"

#include <vulkan/vulkan_xcb.h>

template <>
WindowAdapter::WindowAdapter<xcb_connection_t *,xcb_window_t>( vsg::ref_ptr<vsg::WindowTraits> traits, xcb_connection_t* connection, xcb_window_t window):
    vsg::Window(traits, nullptr)
{
    _nativeAdapter = new NativeAdapterXcb( connection, window );
    _init();
}

NativeAdapterXcb::NativeAdapterXcb( xcb_connection_t* connection, xcb_window_t window ):
    _connection(connection)
  , _window(window)
{}

bool NativeAdapterXcb::valid()
{
    return _connection != nullptr && _window != 0;
}

void NativeAdapterXcb::createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface )
{
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo{};
    surfaceCreateInfo.sType      = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.connection = _connection;
    surfaceCreateInfo.window     = _window;
    vkCreateXcbSurfaceKHR( instance, &surfaceCreateInfo, nullptr, &surface);
}

bool NativeAdapterXcb::pollEvents( vsg::Events & )
{
    return false; 
}

bool NativeAdapterXcb::resized()
{
    return false;
}

bool NativeAdapterXcb::getFramebufferSize( uint32_t &width, uint32_t &height )
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
    return true;
}
