set(SOURCES
    main.cpp
    NativeAdapterXcb.cpp
    NativeAdapterXlib.cpp
    WindowAdapter.cpp
    WindowXcb.cpp
    WindowXlib.cpp
   )


add_executable(winadapt ${SOURCES})

if(USE_XCB)
add_definitions(-DUSE_XCB)
else()
    if(USE_XLIB)
        add_definitions(-DUSE_XLIB)
    else()
        add_definitions(-DUSE_VSG)
    endif()
endif()


target_link_libraries(winadapt vsg::vsg X11 X11-xcb xcb)

