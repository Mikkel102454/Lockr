cmake_minimum_required(VERSION 3.30)
project(Lockr)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lockr)

add_executable(Lockr
        lockr/src/main.cpp
)

target_compile_definitions(shaderEngine PRIVATE UNICODE _UNICODE)
target_link_libraries(shaderEngine PRIVATE engine)