cmake_minimum_required(VERSION 3.20)
project(otuber_windows_preview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(otuber_windows_preview WIN32
    main.cpp
    ../../../src/platform/windows/camera/media_foundation_capture.cpp
    ../../../src/core/motion/camera_tracker.cpp
    ../../../src/core/motion/landmark_mapper.cpp
)

target_include_directories(otuber_windows_preview PRIVATE ../../../src)

if (WIN32)
    target_link_libraries(otuber_windows_preview PRIVATE
        user32
        gdi32
        ole32
        mfplat
        mfreadwrite
        mfuuid
    )
endif()
