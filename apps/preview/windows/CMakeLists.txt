cmake_minimum_required(VERSION 3.20)
project(otuber_windows_preview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(otuber_windows_preview WIN32
    main.cpp
    ../../../src/platform/windows/camera/media_foundation_capture.cpp
    ../../../src/core/motion/camera_tracker.cpp
    ../../../src/core/motion/landmark_mapper.cpp
)

target_include_directories(otuber_windows_preview PRIVATE ../../../src)

target_link_libraries(otuber_windows_preview PRIVATE
    user32
    gdi32
    ole32
    mfplat
    mfreadwrite
    mfuuid
)

find_package(OpenCV QUIET COMPONENTS core imgproc objdetect)
if(OpenCV_FOUND)
    add_executable(otuber_tracking_preview WIN32
        tracking_main.cpp
        ../../../src/platform/windows/camera/media_foundation_capture.cpp
        ../../../src/core/motion/camera_tracker.cpp
        ../../../src/core/motion/landmark_mapper.cpp
        ../../../src/platform/windows/tracking/opencv_face_tracker.cpp
    )
    target_include_directories(otuber_tracking_preview PRIVATE
        ../../../src
        ${OpenCV_INCLUDE_DIRS}
    )
    target_link_libraries(otuber_tracking_preview PRIVATE
        user32
        gdi32
        ole32
        mfplat
        mfreadwrite
        mfuuid
        ${OpenCV_LIBS}
    )
else()
    message(STATUS "OpenCV not found: otuber_tracking_preview will not be built")
endif()
