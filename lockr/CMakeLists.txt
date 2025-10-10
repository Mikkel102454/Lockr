cmake_minimum_required(VERSION 3.5)

# ---------------------------
# Openwall bcrypt (vendored)
# ---------------------------
add_library(bcrypt_ow STATIC
        src/external/libbcrypt/libbcrypt.c
        src/external/libbcrypt/crypt_blowfish.c
        src/external/libbcrypt/crypt_gensalt.c
        src/external/libbcrypt/ow-crypt.c
)
set_target_properties(bcrypt_ow PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(bcrypt_ow
        BEFORE PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/external/libbcrypt
)

if(MSVC)
    target_compile_definitions(bcrypt_ow PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/external/libbcrypt/crypt_blowfish/x86.S")
        target_sources(bcrypt_ow PRIVATE src/external/libbcrypt/crypt_blowfish/x86.S)
    endif()
endif()

# ---------------------------
# Your library
# ---------------------------
add_library(lockr STATIC
        src/utils/string.cpp
        src/utils/config.cpp
        src/utils/env.cpp
        src/class/user.cpp
        src/db.cpp

        src/service/user/create.cpp
        src/service/user/delete.cpp
        src/service/user/exist.cpp
        src/service/user/login.cpp

        src/service/auth/access.cpp
        src/service/auth/refresh.cpp

        src/service/data.cpp

        src/controller/api.cpp
        src/controller/api/user.cpp
        src/controller/api/token.cpp

)
set_target_properties(lockr PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(lockr PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/external
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(lockr PRIVATE CPPHTTPLIB_NO_MMAP)
if(WIN32)
    target_compile_definitions(lockr PRIVATE PLATFORM_WINDOWS UNICODE _UNICODE)
    target_compile_definitions(lockr PUBLIC WIN32_LEAN_AND_MEAN NOMINMAX NOCRYPT)
elseif(APPLE)
    target_compile_definitions(lockr PRIVATE PLATFORM_MACOS)
elseif(UNIX)
    target_compile_definitions(lockr PRIVATE PLATFORM_LINUX)
endif()

# Third-party libs needed by 'lockr'
# (yaml-cpp comes from the parent, bcrypt_ow is vendored here)
target_link_libraries(lockr PUBLIC yaml-cpp bcrypt_ow)

# ---------------------------
# MongoDB (link AFTER 'lockr' exists)
# ---------------------------
find_package(mongocxx CONFIG REQUIRED)
find_package(bsoncxx CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

# ... (everything you already have)

# Link Mongo & OpenSSL to lockr
target_link_libraries(lockr PRIVATE
        mongo::mongocxx_static
        mongo::bsoncxx_static
        OpenSSL::SSL
        OpenSSL::Crypto
)

if (WIN32)
    target_link_libraries(lockr PRIVATE ws2_32 crypt32)
endif()
