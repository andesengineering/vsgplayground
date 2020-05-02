set(SOURCES
    main.cpp
    NativeAdapterXcb.cpp
    NativeAdapterXlib.cpp
    WindowAdapter.cpp
    WindowXcb.cpp
    WindowXlib.cpp
   )

add_definitions(-DUSE_XCB)

add_executable(winadapt ${SOURCES})

target_link_libraries(winadapt vsg::vsg X11 xcb)

