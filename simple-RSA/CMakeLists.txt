cmake_minimum_required(VERSION 3.16)

project(simple_RSA LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(simple_rsa src/main.c)

if (NOT MSVC)
    target_link_libraries(simple_rsa PRIVATE m)
endif()
