set(SOURCES
    main.cpp
    WindowAdapter.cpp
    XCBNativeAdapter.cpp
    XCBWindow.cpp
    XlibNativeAdapter.cpp
    XlibWindow.cpp
   )


add_executable(winadapt ${SOURCES})

add_definitions(-DUSE_XCB)

target_link_libraries(winadapt vsg::vsg X11 X11-xcb xcb)

