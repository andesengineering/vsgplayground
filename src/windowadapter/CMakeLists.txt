set(SOURCES
    main.cpp
    NativeAdapterXcb.cpp
    NativeAdapterXlib.cpp
    WindowAdapter.cpp
    WindowXcb.cpp
    WindowXlib.cpp
   )


add_executable(winadapt ${SOURCES})

add_definitions(-DUSE_XCB)

target_link_libraries(winadapt vsg::vsg X11 X11-xcb xcb)

