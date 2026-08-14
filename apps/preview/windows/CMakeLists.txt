cmake_minimum_required(VERSION 3.20)
project(otuber_windows_preview LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV QUIET COMPONENTS core imgproc objdetect)

add_executable(otuber_windows_preview WIN32
    main.cpp preview_ui.cpp runtime.cpp avatar_image.cpp
    ../../../src/platform/windows/camera/media_foundation_capture.cpp
    ../../../src/core/motion/camera_tracker.cpp
    ../../../src/core/motion/landmark_mapper.cpp
)

target_include_directories(otuber_windows_preview PRIVATE ../../../src .)
target_link_libraries(otuber_windows_preview PRIVATE
    user32 gdi32 comdlg32 ole32
    mf mfplat mfreadwrite mfuuid windowscodecs)

if(OpenCV_FOUND)
    target_sources(otuber_windows_preview PRIVATE
        ../../../src/platform/windows/tracking/opencv_face_tracker.cpp)
    target_include_directories(otuber_windows_preview PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(otuber_windows_preview PRIVATE ${OpenCV_LIBS})
    target_compile_definitions(otuber_windows_preview PRIVATE OTUBER_HAS_OPENCV=1)
else()
    message(STATUS "OpenCV not found: live face tracking is disabled in the main preview")
endif()

if(OpenCV_FOUND)
    add_executable(otuber_tracking_preview WIN32
        tracking_main.cpp
        ../../../src/platform/windows/camera/media_foundation_capture.cpp
        ../../../src/core/motion/camera_tracker.cpp
        ../../../src/core/motion/landmark_mapper.cpp
        ../../../src/platform/windows/tracking/opencv_face_tracker.cpp
    )
    target_include_directories(otuber_tracking_preview PRIVATE ../../../src ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(otuber_tracking_preview PRIVATE
        user32 gdi32 ole32 mf mfplat mfreadwrite mfuuid ${OpenCV_LIBS})
endif()
