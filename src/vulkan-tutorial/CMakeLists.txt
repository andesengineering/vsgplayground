cmake_minimum_required(VERSION 3.7)

project(vulkan-tutorial)


if (VULKAN_SDK)
    set(ENV{VULKAN_SDK} ${VULKAN_SDK})
endif()

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    GWindow.cpp
    GWindowGLFW.cpp
    GWindowXcb.cpp
    GWindowXlib.cpp
    GWindowXmix.cpp
    main.cpp
   )

add_executable(helloAA ${SOURCES})


if( NOT USE_XCB AND NOT USE_XLIB AND NOT USE_GLFW AND NOT USE_XMIX )
    message(FATAL_ERROR "Use one of -DUSE_GLFW=1 , -DUSE_XCB=1, -DUSE_XLIB=1, or -DUSE_XMIX=1")
endif()

if(USE_GLFW)
add_definitions(-DUSE_GLFW)
else()
    if(USE_XCB)
        add_definitions(-DUSE_XCB)
    else()
        if(USE_XLIB)
            add_definitions(-DUSE_XLIB)
        else()
            if(USE_XMIX)
                add_definitions(-DUSE_XMIX)
            endif()
        endif()
    endif()
endif()

target_include_directories(helloAA PUBLIC stb)

target_link_libraries(helloAA vulkan xcb X11-xcb X11 glfw)

add_custom_target(clobber
    COMMAND rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake helloAA Makefile
    )

add_custom_command(TARGET helloAA
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy helloAA ${CMAKE_CURRENT_SOURCE_DIR}
        )




