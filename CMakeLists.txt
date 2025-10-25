cmake_minimum_required(VERSION 3.30)
project(Lockr)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Static MSVC runtime for single-file EXE on Windows
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Prefer static OpenSSL
set(OPENSSL_USE_STATIC_LIBS TRUE)

include(FetchContent)
if(POLICY CMP0126)
    cmake_policy(SET CMP0126 NEW)
endif()

# yaml-cpp via FetchContent
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

# Find mongo *once* here (recommended)
find_package(mongocxx CONFIG REQUIRED)
find_package(bsoncxx CONFIG REQUIRED)

# Add library subdir (it links to yaml-cpp and OpenSSL inside)
add_subdirectory(lockr)

# App executable
add_executable(Lockr lockr/src/main.cpp)
target_link_libraries(Lockr PRIVATE lockr mongo::mongocxx_static mongo::bsoncxx_static)

if (WIN32)
    target_link_libraries(Lockr PRIVATE ws2_32 crypt32)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(lockr PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
    target_compile_definitions(lockr PUBLIC ENV_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")
endif ()

target_compile_definitions(Lockr PRIVATE
        LOCKR_SOURCE_RESOURCES_DIR="${LOCKR_RES_DIR}"
        UNICODE _UNICODE
)
