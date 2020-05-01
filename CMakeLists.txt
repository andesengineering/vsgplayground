cmake_minimum_required(VERSION 3.7)

project(VSGPlayground)

if (VULKAN_SDK)
    set(ENV{VULKAN_SDK} ${VULKAN_SDK})
endif()

find_package(vsg REQUIRED )

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(src/windowadapter)
