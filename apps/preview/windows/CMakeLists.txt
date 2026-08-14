cmake_minimum_required(VERSION 3.20)
project(otuber_windows_preview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(otuber_windows_preview WIN32 main.cpp)
target_include_directories(otuber_windows_preview PRIVATE ../../../src)

if (WIN32)
    target_link_libraries(otuber_windows_preview PRIVATE user32 gdi32)
endif()
