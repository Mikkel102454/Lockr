cmake_minimum_required(VERSION 3.30)
project(Lockr)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static MSVC runtime for a single-file .exe on Windows
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Yaml
include(FetchContent)
if(POLICY CMP0126)
    cmake_policy(SET CMP0126 NEW)
endif()
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

add_subdirectory(lockr)

# App executable
add_executable(Lockr
        lockr/src/main.cpp
)

find_package(mongocxx CONFIG REQUIRED)
find_package(bsoncxx CONFIG REQUIRED)
target_link_libraries(Lockr PRIVATE mongo::mongocxx_static mongo::bsoncxx_static)

# Copy resources next to the binary
set(LOCKR_RES_DIR "${CMAKE_SOURCE_DIR}/lockr/resources")
add_custom_command(TARGET Lockr POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E rm -rf "$<TARGET_FILE_DIR:Lockr>/resources"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${LOCKR_RES_DIR}"
        "$<TARGET_FILE_DIR:Lockr>/resources"
)

target_compile_definitions(Lockr PRIVATE
        LOCKR_SOURCE_RESOURCES_DIR="${LOCKR_RES_DIR}"
        UNICODE _UNICODE
)

# Link the app to your library
target_link_libraries(Lockr PRIVATE lockr)
