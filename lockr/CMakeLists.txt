cmake_minimum_required(VERSION 3.29)

# Build the lockr code as a static library
add_library(lockr STATIC
        src/window.cpp
        src/appLoop.cpp
        src/DirectX12/dxgiFactory.cpp
        src/DirectX12/rtv.cpp
        src/DirectX12/command.cpp
        src/DirectX12/fence.cpp
        src/Pipeline/renderer.cpp
        src/DXWindow.cpp

        include/window.h
        include/appLoop.h
        include/DXWindow.h
)

# Public headers are in lockr/include
target_include_directories(lockr
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Platform-specific defines and libraries
if (WIN32)
    target_compile_definitions(lockr PUBLIC PLATFORM_WINDOWS)
elseif(APPLE)
    target_compile_definitions(lockr PUBLIC PLATFORM_MACOS)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(lockr PUBLIC PLATFORM_LINUX)
endif()
